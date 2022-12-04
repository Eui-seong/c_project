#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

#define INPUT_FILE_NAME "input.txt"
#define OUTPUT_FILE_NAME "output.txt"

typedef struct {
    double *loc;
    int number;
    int cluster;
}Data;


int main() {

    FILE *infp;
    FILE *outfp;
    int d = 0;
    int k = 0;
    int n = 0;

    infp = fopen(INPUT_FILE_NAME, "r");
    outfp = fopen(OUTPUT_FILE_NAME, "w");

    if (infp == NULL)
    {
        printf("\nUnable to open file!\n");
        return 0;
    }

    fscanf(infp,"%d%d%d" ,&k, &d, &n);

    printf("k : %d\nd : %d\nn : %d\n", k, d, n);

    Data *datas = (Data*)malloc(sizeof(Data) * n); // 구조체 n개 동적 할당

    for (int i = 0; i < n; i++) {
        datas[i].loc = (double*)malloc(sizeof(double) * d); // 배열 내 각 구조체의 멤버 변수 loc를 동적 할당
        datas[i].number = i+1;
    }

    // 임시 저장을 위한 2차원 arr배열 생성
    int** arr = (int**)malloc(sizeof(int*) * n);
    for (int i = 0; i < n; i++){
        arr[i] = (int*)malloc(sizeof(int) * d);
    }

    // 파일 각 라인의 위치 값을 임시 arr 배열에 저장
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < d; j++) {
            fscanf(infp,"%d", &arr[i][j]);
        }
    }

    // arr 배열에 저장 되어 있는 값들을 구조체 멤버 변수 배열로 이동
    printf("\n각 데이터의 좌표 확인\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < d; j++) {
            datas[i].loc[j] = arr[i][j];
            printf("%f ", datas[i].loc[j]);
        }
        printf("\n");
    }

    srand((unsigned int)time(NULL)); // 난수 생성을 위한 코드

    // 중복 없는 난수 배열 생성
    int a[k];

    for (int i = 0; i < k; i++) {
        a[i] = rand() % n;
        for (int j = 0; j < i; j++) {
            if (a[i] == a[j]) i--;
        }
    }

//    // 난수 배열 결과 출력용 코드
    printf("\n난수 배열 값 확인\n");
    for (int i = 0; i < k; i++) {
        printf("%d\n", a[i]);
    }

    // centroid 값 저장을 위한 2차원 배열 동적 할당
    // centroid 값 랜덤 지정
    double** centroid = (double**)malloc(sizeof(double*) * k);
    for (int i = 0; i < k; i++){
        centroid[i] = (double*)malloc(sizeof(double) * d);
        centroid[i] = datas[a[i]].loc;
    }

    // centroid 값 출력용 코드
    printf("\ncentroid check\n");
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < d; j++) {
            printf("%f ", centroid[i][j]);
        }
        printf("\n");
    }

    // 거리 계산 결과 저장을 위한 2차원 배열 동적 할당
    double** distance = (double**)malloc(sizeof(double*) * k);
    for (int i = 0; i < k; i++){
        distance[i] = (double*)malloc(sizeof(double) * n);
    }

    double temp = 0;

    // 거리 계산 코드
    for (int num = 0; num < k; num++){
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < d; j++) {
                temp += pow(centroid[num][j] - datas[i].loc[j], 2);
            }
            distance[num][i] = sqrt(temp);
            temp = 0;
        }
    }

    // 거리 계산 확인용 코드
    printf("\ndistance check\n");
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < n; j++) {
            printf("%f ", distance[i][j]);
        }
        printf("\n");
    }


    double distance_temp = 0.0;

    // 거리 비교 후, 각 데이터의 cluster 값 지정
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < k; j++) {
            if (j == 0) {
                distance_temp = distance[j][i];
                datas[i].cluster = j;
            }
            else {
                if (distance_temp > distance[j][i]) {
                    distance_temp = distance[j][i];
                    datas[i].cluster = j;
                }
            }
        }
    }

    for (int i = 0; i < n; i++) {
        printf("%d ", datas[i].cluster);
    }
    printf("\n");

    int iter = 1;

    // 출력을 위한 배열 생성
    int** output_arr = (int**)malloc(sizeof(int*) * k);
    for (int i = 0; i < k; i++){
        output_arr[i] = (int*)malloc(sizeof(int) * n);
    }
    
    for (int i = 0; i < n; i++) {
        output_arr[datas[i].cluster][i] = datas[i].number;
    }
    
    fprintf(outfp, "%d\n", iter);
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < n; j++) {
            if (output_arr[i][j] != 0) {
                fprintf(outfp, "%d ", output_arr[i][j]);
            }
        }
        fputc('\n', outfp);
    }
    fputc('\n', outfp);



    double check[k][d]; // centroid 값 임시 저장
    
    bool is_same = false;

    
    
    // iter 반복
    while (true) {

        // output_arr 0으로 초기화
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < n; j++) {
                output_arr[i][j] = 0;
            }
        }
        
        
        // check에 이전 centroid 값 재 지정
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < d; j++) {
                check[i][j] = centroid[i][j];
            }
        }


        // centroid 값 0으로 설정
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < d; j++) {
                centroid[i][j] = 0;
            }
        }

        // centroid 평균 계산용 개수 세기
        int count_data[k];

        for (int i = 0; i < n; i++) {
            count_data[datas[i].cluster]++;
        }

        // centroid 값 재 지정

        // 1. centroid 값 모두 더하기
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < d; j++) {
                centroid[datas[i].cluster][j] += datas[i].loc[j];
            }
        }

        // 2. centroid 값 평균 구하기
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < d; j++) {
                centroid[i][j] /= count_data[i];
            }
        }
        
        printf("\ncentroid\n");
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < d; j++) {
                printf("%f ", centroid[i][j]);
            }
            printf("\n");
        }
        
        for(int i =0 ; i< k ; i++){
            for(int j = 0 ; j<d ;j++){
                printf("//////////");
                printf("centroid : %f\n", centroid[i][j]);
                printf("check : %f\n", check[i][j]);

            }
        }
        
        printf("\n");
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < d; j++) {
                if (centroid[i][j] == check[i][j]){
                    is_same = true;
                }
                else {
                    is_same = false;
                    break;
                }
                printf("centroid : %f\n", centroid[i][j]);
                printf("check : %f\n", check[i][j]);
            }
            printf("\n");
        }
        printf("%s\n", is_same ? "true" : "false");


        if (is_same || iter > 10) {  // 이전 centroid 값과 모두 동일 하다면
            break;
        }

        else {
            // 거리 계산
            for (int num = 0; num < k; num++){
                for (int i = 0; i < n; i++) {
                    for (int j = 0; j < d; j++) {
                        temp += pow(centroid[num][j] - datas[i].loc[j], 2);
                    }
                    distance[num][i] = sqrt(temp);
                    temp = 0;
                }
            }

            // cluster 지정하기
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < k; j++) {
                    if (j == 0) {
                        distance_temp = distance[j][i];
                        datas[i].cluster = j;
                    }
                    else {
                        if (distance_temp > distance[j][i]) {
                            distance_temp = distance[j][i];
                            datas[i].cluster = j;
                        }
                    }
                }
            }
            iter++;
        }
        
        printf("iter : %d\n", iter);
        
        printf("\ndistance check\n");
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < n; j++) {
                printf("%f ", distance[i][j]);
            }
            printf("\n");
        }

        
        
        printf("\nCluster check\n");
        for (int i = 0; i < n; i++) {
            printf("%d ", datas[i].cluster);
        }
        printf("\n");
        

        // 파일 출력
        if (!is_same) {
            
            for (int i = 0; i < n; i++) {
                output_arr[datas[i].cluster][i] = datas[i].number;
            }

            fprintf(outfp, "%d\n", iter);
            for (int i = 0; i < k; i++) {
                for (int j = 0; j < n; j++) {
                    if (output_arr[i][j] != 0) {
                        fprintf(outfp, "%d ", output_arr[i][j]);
                    }
                }
                fputc('\n', outfp);
            }
            fputc('\n', outfp);
        }
    }

    fclose(infp);
    fclose(outfp);

}

