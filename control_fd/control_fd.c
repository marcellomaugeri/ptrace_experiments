// Author: Marcello Maugeri
// Brief: This program shows how to overwrite file descriptors in a child process using ptrace


#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/syscall.h>
#include<sys/ptrace.h>
#include<sys/wait.h>
#include<stdint.h>

int main(){
    //Open two file descriptors in /tmp
    int fd1 = open("/tmp/test1", O_CREAT | O_RDWR | O_TRUNC, 0666);
    int fd2 = open("/tmp/test2", O_CREAT | O_RDWR | O_TRUNC, 0666);

    //Create fd3
    int fd3 = open("/tmp/test3", O_CREAT | O_RDWR | O_TRUNC, 0666);

    // dup2 fd3 into fd2
    if ((fd3 = dup2(fd2, fd3)) == -1) {
        perror("dup2");
        return 1;
    }
    
    //Write in fd1
    char str1[5] = "Fd1!\0";
    write(fd1, str1, strlen(str1));

    //Write in fd2
    char str2[5] = "Fd2!\0";
    write(fd2, str2, strlen(str2));

    //Flush fd1 and fd2
    fsync(fd1);
    fsync(fd2);
    fsync(fd3);

    //Read fd1
    lseek(fd1, 0, SEEK_SET);
    char buf1[5];
    read(fd1, buf1, 5);

    //Read fd2
    lseek(fd2, 0, SEEK_SET);
    char buf2[5];
    read(fd2, buf2, 5);

    //Read fd3
    lseek(fd3, 0, SEEK_SET);
    char buf3[5];
    read(fd3, buf3, 5);

    //Print fd1, fd2 and fd3
    printf("Parent's fd1: %p with content: %s\n", &fd1, buf1);
    printf("Parent's fd2: %p with content: %s\n", &fd2, buf2);
    printf("Parent's fd3: %p with content: %s\n", &fd3, buf3);

    //Fork
    pid_t pid = fork();
    //Child
    if(pid == 0){
        //Make it attachable with PTRACE_ME
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        //Stop itself
        raise(SIGSTOP);

        //Read fd1
        lseek(fd1, 0, SEEK_SET);
        read(fd1, buf1, 5);

        //Read fd2
        lseek(fd2, 0, SEEK_SET);
        read(fd2, buf2, 5);

        //Read fd3
        lseek(fd3, 0, SEEK_SET);
        read(fd3, buf3, 5);

        //Print fd1, fd2 and fd3
        printf("Child's fd1: %p, number %d, with content: %s\n", &fd1, fd1, buf1);
        printf("Child's fd2: %p, number %d, with content: %s\n", &fd2, fd2, buf2);
        printf("Child's fd3: %p, number %d, with content: %s\n", &fd3, fd3, buf3);
    } else {
        //Attach to child
        ptrace(PTRACE_ATTACH, pid, NULL, NULL);
        //Wait for child to stop
        waitpid(pid, NULL, 0);

        //Overwrite fd3 with fd1
        ptrace(PTRACE_POKEDATA, pid, (void*)((unsigned long)&fd3), (void*)((unsigned long) fd1));

        //Continue child
        ptrace(PTRACE_CONT, pid, NULL, NULL);
        
        //Wait for child to end
        waitpid(pid, NULL, 0);
    }
}