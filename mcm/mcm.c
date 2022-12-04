#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define INPUT_FILE_NAME "test_set.txt"
#define OUTPUT_FILE_NAME "test_set_sol.txt"

// 재귀적으로 행렬 최소곱을 찾기 위한 알고리즘 함수
int find(int*arr, int min_count, int max_count)
{
    if (min_count == max_count)
    {
        return 0;
    }
    
    int temp;
    
    int min = 2147483647; // 최소 값을 가장 큰 int 값으로 지정
    
    int value;
    
    for (int i = min_count; i < max_count; i++)
    {
        temp = arr[min_count - 1] * arr[i] * arr[max_count];
        value = + find(arr, min_count, i) + find(arr, i + 1, max_count) + temp;
        
        if (min > value)
        {
            min = value;
        }
    }
    return min;
}

int main()
{

    FILE *infp;
    FILE *outfp;
    int line = 0;
    int row = 0;
    int col = 0;
    char ch = '0';

    infp = fopen(INPUT_FILE_NAME, "r");
    outfp = fopen(OUTPUT_FILE_NAME, "w");

    if (infp == NULL)
    {
        printf("\nUnable to open file!\n");
        return 0;
    }


    while (ch != EOF)
    {
        ch = fgetc(infp);
        if (ch == '\n')
        {
            line++;
        }
    }


    // 모든 행렬의 행, 열 값을 담은 2차원 배열
    int **row_col = (int**)malloc(sizeof(int*) * line);
    for(int i=0; i<line; i++)
    {
        row_col[i] = (int*) malloc ( sizeof(int) * 2);
    }

    fseek( infp, 0L, SEEK_SET );

    // 파일에서 행, 열 값 읽어와서 row_col에 저장
    // index 0 : 행 , index 1 : 열
    for (int i = 0; i < line; i++)
    {
        fscanf(infp, "%d%d", &row, &col);
        row_col[i][0] = row;
        row_col[i][1] = col;
    }

    /*
     3 2
     2 6
     6 5
     5 4 -> [3, 2, 6, 5, 4]
     */
    // 행렬 최소곱을 구하기 위한 배열

    int size_of_array = line + 1;
    int *array = (int*)malloc(sizeof(int) * size_of_array);


    // array 인덱스 범위 0 ~ 4
    // 행렬 최소곱 구하기 연산을 위한 배열 값 대입
    array[0] = row_col[0][0];

    for (int i = 1; i < size_of_array; i++)
    {
        array[i] = row_col[i-1][1];
    }

    // 행렬 최소곱 값 구하기
    int min_value;
    min_value = find(array, 1, size_of_array - 1);

    // 3차원 배열에 모든 행렬 담고, 랜덤으로 값 할당
    int ***array_3d = (int***)malloc(sizeof(int**) * line);
    for (int i = 0; i < line; i++) {
        array_3d[i] = (int**)malloc(sizeof(int*) * row_col[i][0]);
        for (int j = 0; j < row_col[i][0]; j++) {
            array_3d[i][j] = (int*)malloc(sizeof(int) * row_col[i][1]);
        }
    }

    srand((unsigned int)time(NULL)); // 난수 생성을 위한 코드

    int random;

    for (int i = 0; i < line; i++)
    {
        for (int j = 0; j < row_col[i][0]; j++)
        {
            for (int k = 0; k < row_col[i][1]; k++)
            {
                random = rand() % 19 + 1;
                array_3d[i][j][k] = random;
            }
        }
    }


    /*
     5 3
     5 4
     5 7
     5 2
     5 5
     */
    // 행렬 곱 연산을 위한 3차원 배열
    int ***temp_3d = (int***)malloc(sizeof(int**) * line-1);
    for (int i = 0; i < line-1; i++){
        temp_3d[i] = (int**)malloc(sizeof(int*) * row_col[0][0]);
        for (int j = 0; j < row_col[0][0]; j++) {
            temp_3d[i][j] = (int*)malloc(sizeof(int) * row_col[i+1][1]);
        }
    }



    // 첫번째 행렬 연산 결과 저장
    int temp_val = 0;

    for (int i = 0; i < row_col[0][0]; i++)
    {
        for (int j = 0; j < row_col[1][1]; j++)
        {
            for (int k = 0; k < row_col[0][1]; k++)
            {
                temp_val += array_3d[0][i][k] * array_3d[1][k][j];
            }
            temp_3d[0][i][j] = temp_val;
            temp_val = 0;
        }
    }

    // 행렬 연산
    for (int i = 1; i < line-1; i++)
    {
        for (int j = 0; j < row_col[0][0]; j++)
        {
            for (int k = 0; k < row_col[i+1][1]; k++)
            {
                for (int l = 0; l < row_col[i][1]; l++)
                {
                    temp_val += temp_3d[i-1][j][l] * array_3d[i+1][l][k];
                }
                temp_3d[i][j][k] = temp_val;
                temp_val = 0;
            }
        }
    }
    
    // 연산 결과 행렬의 행, 열 값
    int result_row = row_col[0][0];
    int result_col = row_col[line-1][1];
    
    // 읽어온 행,열 값 확인 코드
    printf("test code row,col\n");
    for (int i = 0; i < line; i++)
    {
        printf("%d", row_col[i][0]);
        printf(" %d\n", row_col[i][1]);
    }
    
    // 생성된 행렬 확인 코드
    printf("\nGenerated Matrix\n");
    for (int i = 0; i < line; i++)
    {
        for (int j = 0; j < row_col[i][0]; j++)
        {
            for (int k = 0; k < row_col[i][1]; k++)
            {
                printf("%d ", array_3d[i][j][k]);
            }
            printf("\n");
        }
        printf("\n");
    }
    
    // 행렬 최소곱 값 확인 코드
    printf("\nmin_value is: %d\n", min_value);
    
    // 행렬 계산 결과 확인용 코드
    printf("\nResult Matrix\n");
    for (int i = 0; i < result_row; i++)
    {
        for (int j = 0; j < result_col; j++)
        {
            printf("%d ",temp_3d[line-2][i][j]);
        }
        printf("\n");
    }
    
    // 필요한 내용 Output 파일에 출력
    fprintf(outfp, "%d\n", min_value);

    fprintf(outfp, "\n%s\n", "Output Matrix");
    for (int i = 0; i < result_row; i++)
    {
        for (int j = 0; j < result_col; j++)
        {
            fprintf(outfp, "%d ", temp_3d[line-2][i][j]);
        }
        fputc('\n', outfp);
    }
    fputc('\n', outfp);



    for (int i = 0; i < line; i++)
    {
        fprintf(outfp, "%s %d\n", "Input Matrix", i+1);
        for (int j = 0; j < row_col[i][0]; j++)
        {
            for (int k = 0; k < row_col[i][1]; k++)
            {
                fprintf(outfp, "%d ", array_3d[i][j][k]);
            }
            fputc('\n', outfp);
        }
        fputc('\n', outfp);
    }


    // 사용한 메모리 해제

    for(int i = 0; i < line; i++)
    {
        for(int j=0; j < row_col[i][0]; j++)
        {
            free(array_3d[i][j]);
        }
        free(array_3d[i]);
    }
    free(array_3d); // 3차원 : array_3d


    for(int i = 0; i < line - 1; i++)
    {
        for(int j=0; j < row_col[0][0]; j++)
        {
            free(temp_3d[i][j]);
        }
        free(temp_3d[i]);
    }
    free(temp_3d); // 3차원 : temp_3d;



    for(int i=0; i < line; i++)
    {
        free(row_col[i]);
    }
    free(row_col); // 2차원 : row_col


    free(array); // 1차원 : array


    // 사용한 파일 닫기
    fclose(infp);
    fclose(outfp);
}






