#include "common.h"
#include "../mytools.h"

void
send_files(const int sock, char** files, const size_t files_count);

int
main(size_t argc, char** argv)
{
    struct sockaddr_in addr;
    int sockfd = 0;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    ERR_CHECK(sockfd, -1)

    if (inet_pton(AF_INET, HOST, &addr.sin_addr) <= 0) {
        printf("inet_pton error: %s\n", strerror(errno));
        exit(-1);
    }

    TRY(connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)))

// #####
    send_files(sockfd, (argv + 1), argc - 1);

    for (size_t n = 0; n < argc - 1; n++) {
        size_t len = 0;
        ssize_t bytes = 0;
        char buf[BUF_SIZE] = {0};

        printf("reading from server...\n");

        TRY(read(sockfd, &len, sizeof(len)))

        bytes = read(sockfd, buf, len);
        ERR_CHECK(bytes, -1)
    
        printf("msg from server[%ld]: %s\n", bytes, buf);
    }

// #####

    TRY(close(sockfd));

    return 0;
}

void
send_files(const int sock, char** files, const size_t files_count)
{
    TRY(write(sock, &files_count, sizeof(files_count)))

    for (size_t n = 0; n < files_count; n++) {
        size_t len = strlen(files[n]);

        TRY(write(sock, &len, sizeof(len)))
        TRY(write(sock, files[n], len))

        printf("sended: %s\n", files[n]);
    }
}
