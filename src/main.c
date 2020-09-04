#include <uart.h>


#include <stdio.h>

int main() {
    /* int fd = open("/dev/serial0", O_RDWR); */
    /* int r = get_int(fd, "1399"); */
    /* printf("%d\n", r); */
    printf("0 - Get Integer     1 - Get Float   2 - Get String\n3 - Send Integer        4 - Send Float  5 - Send String\n");
    printf("Choose your action: ");
    
    int option;
    scanf("%d", &option);

    switch(option) {
        case 0:
            printf("\nResponse: %d\n", get_int());
            break;
        case 1:
            printf("\nResponse: %f\n", get_float());
            break;
        case 2:
            printf("\nResponse: %s\n", get_str());
            break;
        default:
            printf("tt\n");
    }
    return 0;
}
