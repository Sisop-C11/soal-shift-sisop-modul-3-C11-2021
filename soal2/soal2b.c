#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
typedef long long ll;

pthread_t tid[4];
int matriks1[4][6], matriks2[4][6];
ll matriks_hasil[4][6];

ll fact (int num) {
        if (num == 1 || num == 0) return 1;
        return num * fact(num - 1);        
}

ll count (int val1, int val2) {
        if (val1 == 0 || val2 == 0) return 0;
        else if (val1 >= val2) return fact(val1) / fact(val1 - val2);
        else return fact(val1);
}

void* count_thread (void* argv) {
        pthread_t id = pthread_self();
        int it;

        if (pthread_equal(id, tid[0])) {
                for (it = 0; it < 6; it++) {
                        matriks_hasil[0][it] = count(matriks1[0][it], matriks2[0][it]);
                }
        } else if (pthread_equal(id, tid[1])) {
                for (it = 0; it < 6; it++) {
                        matriks_hasil[1][it] = count(matriks1[1][it], matriks2[1][it]);
                }
        } else if (pthread_equal(id, tid[2])) {
                for (it = 0; it < 6; it++) {
                        matriks_hasil[2][it] = count(matriks1[2][it], matriks2[2][it]);
                }
        } else if (pthread_equal(id, tid[3])) {
                for (it = 0; it < 6; it++) {
                        matriks_hasil[3][it] = count(matriks1[3][it], matriks2[3][it]);
                }
        }

        return NULL;
}

void main () {
        key_t key = 1234;
        int (*hasil)[6];
        int row, col, temp;
        int i, err;

        int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
        hasil = shmat(shmid, NULL, 0);

        printf("Matriks A:\n");
        for (row = 0; row < 4; row++) {
                for (col = 0; col < 6; col++) {
                        matriks1[row][col] = hasil[row][col];
                        printf("%d ", matriks1[row][col]);
                }
                puts("");
        }
        puts("");

        printf("Masukkan nilai matriks B:\n");
        for (row = 0; row < 4; row++) {
                for (col = 0; col < 6; col++) {
                        scanf("%d", &matriks2[row][col]);
                }
        }

        for (i = 0; i < 4; i++) {
                err = pthread_create(&(tid[i]), NULL, &count_thread, NULL);

        }
        pthread_join(tid[0], NULL);
        pthread_join(tid[1], NULL);
        pthread_join(tid[2], NULL);
        pthread_join(tid[3], NULL);

        printf("Hasil perhitungan:\n");
        for (row = 0; row < 4; row++) {
                for (col = 0; col < 6; col++) {
                        printf("%lld ", matriks_hasil[row][col]);
                }
                puts("");
        }
        shmdt(hasil);
        shmctl(shmid, IPC_RMID, NULL);
}
