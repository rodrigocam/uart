#include <uart.h>


#include <stdio.h>

int main() {
    printf("0 - Get Integer     1 - Get Float   2 - Get String\n3 - Send Integer        4 - Send Float  5 - Send String\n");
    printf("Choose your action: ");
    
    int option = -1;
    scanf("%d", &option);

    char c_content[255];
    int i_content;
    float f_content;

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
        case 3:
            scanf("%d", &i_content);
            send_int(&i_content);
            break;
        case 4:
            scanf("%f", &f_content);
            send_float(&f_content);
            break;
        case 5:
            printf("\nType your message: ");

            // We need this to flush the newline typed after scanf
            getc(stdin);
            
            fgets(c_content, 255, stdin);
            printf("c_content: %s\n", c_content);
            send_str(c_content, 255);
            break;
        default:
            printf("tt\n");
    }
    return 0;
}
