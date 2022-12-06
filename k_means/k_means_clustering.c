#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

#define INPUT_FILE_NAME "input.txt"
#define OUTPUT_FILE_NAME "input_sol.txt"

int main() {
    
    FILE *infp;
    FILE *outfp;
    int k, d, n;
    
    infp = fopen(INPUT_FILE_NAME, "r");
    outfp = fopen(OUTPUT_FILE_NAME, "w");
    
    if (infp == NULL)
    {
        
        printf("\nUnable to open file!\n");
        return 0;
    }
    
    fscanf(infp,"%d%d%d" ,&k, &d, &n);
    
    // 입력 받은 k, d, n 값 확인
    printf("Kluster:%d  Dimension:%d  Number:%d\n", k, d, n);
    
    // 데이터 좌표 입력을 위한 2차원 배열 동적 할당
    double **data_array = (double**)malloc(sizeof(double*) * n);
    for (int i = 0; i < n; i++) {
        data_array[i] = (double*)malloc(sizeof(double) * d);
    }
    
    // 파일의 데이터 좌표 읽어와서 2차원 배열에 저장
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < d; j++) {
            fscanf(infp, "%lf", &data_array[i][j]);
        }
    }
    
    fclose(infp);
    
    // centroid 값 저장을 위한 2차원 배열 동적 할당
    double** centroid = (double**)malloc(sizeof(double*) * k);
    for (int i = 0; i < k; i++){
        centroid[i] = (double*)malloc(sizeof(double) * d);
    }
    
    srand((unsigned int)time(NULL)); // 난수 생성을 위한 코드
    
   // 중복 없는 난수 배열 생성
    int random[k];

    for (int i = 0; i < k; i++) {
        random[i] = rand() % n;
        for (int j = 0; j < i; j++) {
            if (random[i] == random[j]) i--;
        }
    }
    
    // 난수 배열 결과 출력용 코드
    printf("\n난수 배열 값 확인\n");
    for (int i = 0; i < k; i++) {
        printf("%d\n", random[i]);
    }

    // centroid 값 랜덤 지정
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < d; j++) {
            centroid[i][j] = data_array[random[i]][j];
        }
    }
    
    // centroid 값 출력용 코드
    printf("\nIter 1 centroid\n");
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < d; j++) {
            printf("%f ", centroid[i][j]);
        }
        printf("\n");
    }
    
    // 클러스터 번호 지정을 위한 배열
    int* cluster = (int*)malloc(sizeof(int) * n);
    
    double min_dist = 0;
    double dist = 0;
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < k; j++) {
            for (int l = 0; l < d; l++) {
                dist += pow(centroid[j][l] - data_array[i][l], 2);
            }
            if (j == 0) {
                min_dist = dist;
                cluster[i] = j;
            }
            else {
                if (dist < min_dist) {
                    min_dist = dist;
                    cluster[i] = j;
                }
            }
            dist = 0;
        }
    }
    
    
    int iter = 1;
    
    
    
    fprintf(outfp, "%d\n", iter);
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < n; j++) {
            if (cluster[j] == i) {
                fprintf(outfp, "%d ", j+1);
            }
        }
        fputc('\n', outfp);
    }
    fputc('\n', outfp);
    
    
    double check[k][d]; // centroid 값 임시 저장
    
    while (true) {
    
        
        double sum = 0;
        
        int is_same = 0;
        
        // check에 이전 centroid 값 임시 저장
        iter++;
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < d; j++) {
                check[i][j] = centroid[i][j];
            }
        }
        
        int count = 0;
        
        // centroid 값 0으로 설정
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < d; j++) {
                centroid[i][j] = 0;
            }
        }
        
        // centroid 계산
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < n; j++) {
                if (cluster[j] == i) {
                    for (int l = 0; l < d; l++) {
                        centroid[i][l] += data_array[j][l];
                    }
                    count++;
                }
            }
            for (int m = 0; m < d; m++) {
                centroid[i][m] /= count;
            }
            count = 0;
            sum = 0;
        }
        
        printf("\nIter %d centroid\n", iter);
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < d; j++) {
                printf("%f ", centroid[i][j]);
            }
            printf("\n");
        }
        
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < d; j++) {
                if (check[i][j] == centroid[i][j]) {
                    is_same++;
                }
            }
        }
        
        if (is_same == k * d || iter > 20) {
            break;
        }

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < k; j++) {
                for (int l = 0; l < d; l++) {
                    dist += pow(centroid[j][l] - data_array[i][l], 2);
                }
                if (j == 0) {
                    min_dist = dist;
                    cluster[i] = j;
                }
                else {
                    if (dist < min_dist) {
                        min_dist = dist;
                        cluster[i] = j;
                    }
                }
                dist = 0;
            }
        }
        
        fprintf(outfp, "%d\n", iter);
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < n; j++) {
                if (cluster[j] == i) {
                    fprintf(outfp, "%d ", j+1);
                }
            }
            fputc('\n', outfp);
        }
        fputc('\n', outfp);
    }
    
    
    for (int i = 0; i < n; i++){
        free(data_array[i]);
    }
    free(data_array);

    for (int i = 0; i < k; i++){
        free(centroid[i]);
    }
    free(centroid);
    
    free(cluster);
    
    fclose(outfp);
    
}
