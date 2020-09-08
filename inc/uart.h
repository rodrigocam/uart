#include <unistd.h> // Used for UART
#include <fcntl.h> // Used for UART
#include <termios.h> // Used for UART

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define CODE_GET_INT 0xA1
#define CODE_GET_FLOAT 0xA2
#define CODE_GET_STR 0xA3

#define CODE_SEND_INT 0xB1
#define CODE_SEND_FLOAT 0xB2
#define CODE_SEND_STR 0xB3

#define DEV_FILE "/dev/serial0"

const unsigned char CODE_MAT[4] = {1, 3, 9, 9};


typedef union {
    struct {
        unsigned char cmd;
        unsigned char payload[4];
        unsigned char auth[4];
    } data;
    unsigned char bytes[9];
} SendPackage;


void get_generic(char* fd_path, char CODE, int response_s, void* response, int comp) {
    int fd = -1;
    fd = open(fd_path, O_RDWR | O_NOCTTY);
   
    if(fd == -1) {
        fprintf(stderr, "Failed to open `%s`\n", fd_path);
        exit(1);
    }

    struct termios options;
    
    tcgetattr(fd, &options);
    options.c_cflag = B115200 | CS8 | CLOCAL | CREAD;     //<Set baud rate
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &options);

    char msg[5] = {CODE};
    strcat(msg, CODE_MAT);
    
    const int writed_s = write(fd, &msg[0], sizeof(msg));
   
    if(writed_s < 0) {
        fprintf(stderr, "Failed to get_generic, error number %d:  `%s`\n", errno, strerror(errno));
        close(fd);
        exit(1);
    }
    
    if(writed_s < sizeof(msg)) {
        fprintf(stderr, "Failed to get_generic, impossible to write entire message content\n");
        close(fd);
        exit(1);
    }

    // We have to wait UART communication
    sleep(2);
    int read_s = read(fd, response, response_s);

    if(read_s < 0) {
        fprintf(stderr, "Failed to get_generic, error number %d:  `%s`\n", errno, strerror(errno));
        close(fd);
        exit(1);
    }

    if(read_s < comp) {
        fprintf(stderr, "Failed to get_generic, impossible to read entire response. Specified size: %d, Received size: %d\n", response_s, read_s);
        close(fd);
        exit(1);
    }
    
    close(fd);
}

int get_int() {
    char response[4];

    get_generic(DEV_FILE, CODE_GET_INT, sizeof(int), &response, sizeof(int));
    
    return *((int*) response);
}

float get_float() {
    char response[4];

    get_generic(DEV_FILE, CODE_GET_FLOAT, sizeof(float), &response, sizeof(float));

    return *((float*) response);
}

char* get_str() {
    char response[255];

    get_generic(DEV_FILE, CODE_GET_STR, sizeof(response), &response, -1);

    short str_size = (short) response[0];
    char* str = malloc(str_size);

    for(short i=0; i<str_size; i++) {
        str[i] = response[i+1];
    }

    return str;
}

void generic_send(char CODE, void* content, int content_size, void* response, int response_size) {
    int fd = -1;
    fd = open(DEV_FILE, O_RDWR | O_NOCTTY);
   
    if(fd == -1) {
        fprintf(stderr, "Failed to open `%s`\n", DEV_FILE);
        exit(1);
    }

    struct termios options;
    
    tcgetattr(fd, &options);
    options.c_cflag = B115200 | CS8 | CLOCAL | CREAD;     //<Set baud rate
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &options);

    SendPackage package;
    package.data.cmd = CODE;
    memcpy(package.data.auth, CODE_MAT, 4);
    memcpy(package.data.payload, content, 4);

    const int writed_s = write(fd, package.bytes, 9);
   
    if(writed_s < 0) {
        fprintf(stderr, "Failed to get_generic, error number %d:  `%s`\n", errno, strerror(errno));
        close(fd);
        exit(1);
    }

    sleep(2);
    int read_s = read(fd, response, response_size);

    if(read_s < 0) {
        fprintf(stderr, "Failed to get_generic, error number %d:  `%s`\n", errno, strerror(errno));
        close(fd);
        exit(1);
    }

    /* printf("response: %d\n", *(int*) response); */
    close(fd);
}

void send_str(char* content, int content_size) {
    /* generic_send(CODE_SEND_STR, content, content_size); */
}

void send_int(void* content) {
    char response[4];
    generic_send(CODE_SEND_INT, content, sizeof(int), response, sizeof(int));
    printf("Response: %d\n", *(int*) response);
}

void send_float(void* content) {
    char response[4];
    generic_send(CODE_SEND_FLOAT, content, sizeof(float), response, sizeof(float));
    printf("Response: %f\n", *(float*) response);
}
