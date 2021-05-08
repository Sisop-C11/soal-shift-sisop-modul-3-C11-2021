#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

void main () {
        key_t key = 1234;
        int matriks1[4][3], matriks2[3][6];
        int (*hasil)[6];
        int row, col, temp;

        int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
        hasil = shmat(shmid, NULL, 0);

        printf("Masukkan nilai matriks 1:\n");
        for (row = 0; row < 4; row++) {
                for (col = 0; col < 3; col++) {
                        scanf("%d", &matriks1[row][col]);
                }
        }

        printf("\nMasukkan nilai matriks 2:\n");
        for (row = 0; row < 3; row++) {
                for (col = 0; col < 6; col++) {
                        scanf("%d", &matriks2[row][col]);
                }
        }

        printf("\nNilai matriks hasil perkalian:\n");
        for (row = 0; row < 4; row++) {
                for (col = 0; col < 6; col++) {
                        int val = 0;
                        for (temp = 0; temp < 3; temp++) {
                                val += matriks1[row][temp] + matriks2[temp][col];
                        }
                        hasil[row][col] = val;
                        printf("%d ", val);
                }
                puts("");
        }

        sleep(10);
        shmdt(hasil);
        shmctl(shmid, IPC_RMID, NULL);
}
