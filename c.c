#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <pthread.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SIZE 10
#define PORT 8080

// Function to run in a thread
void *thread_function(void *arg)
{
    printf("Thread running...\n");

    // Perform thread-specific tasks here...

    pthread_exit(NULL);
}

// Simple TCP server
void run_server()
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    // Handle client connections...
    close(new_socket);

    printf("Server stopped\n");
}

int main()
{
    // Memory manipulation with mmap
    void *mem_block = mmap(NULL, SIZE * sizeof(int), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);

    if (mem_block == MAP_FAILED)
    {
        perror("mmap");
        return 1;
    }

    int *nums = (int *)mem_block;
    for (int i = 0; i < SIZE; ++i)
    {
        nums[i] = i;
    }

    for (int i = 0; i < SIZE; ++i)
    {
        printf("%d ", nums[i]);
    }
    printf("\n");

    munmap(mem_block, SIZE * sizeof(int));

// Inline Assembly
#if defined(__x86_64__) || defined(__i386__)
    unsigned int eax;
    unsigned int ebx;
    unsigned int ecx;
    unsigned int edx;
    char vendor[13];

    eax = 0; // get vendor string

    __asm__("cpuid"
            : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
            : "0"(eax));

    memcpy(vendor, &ebx, 4);
    memcpy(vendor + 4, &edx, 4);
    memcpy(vendor + 8, &ecx, 4);
    vendor[12] = '\0';

    printf("CPU Vendor: %s\n", vendor);
#endif

    // Multithreading
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, thread_function, NULL);
    pthread_join(thread_id, NULL);

    // Network programming (run a simple server)
    run_server();

    return 0;
}
