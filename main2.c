#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define LOTTO_COUNT     6       // 選幾個號碼
#define LOTTO_RANGE     49      // 1~49
#define COUNTER_FILE    "counter.dat"
#define LOTTO_FILE      "lotto.txt"

// 函數宣告
void init_file();
int get_counter();
void set_counter(int counter);
void print_lottofile(int num_set, int counter, const char *lotto_file);
int num_in_numset(int num, int numset[]);
void output_lottorow(FILE *fp, int numset[]);

int main() {
    srand(time(NULL));  // 只播種一次！

    int counter;
    init_file();
    counter = get_counter();
    printf("counter = %d\n", counter);

    // 假設 do_lotto_main 會呼叫 print_lottofile
    print_lottofile(1, counter, LOTTO_FILE);  // 產生一組號碼

    set_counter(counter + 1);  // 更新計數
    return 0;
}

// 初始化計數檔案（若不存在則建立）
void init_file() {
    FILE *fp = fopen(COUNTER_FILE, "ab+");
    if (fp == NULL) {
        perror("無法建立 counter.dat");
        exit(1);
    }
    fclose(fp);
}

// 讀取執行次數
int get_counter() {
    int counter = 0;
    FILE *fp = fopen(COUNTER_FILE, "rb");
    if (fp == NULL) {
        return 0;  // 檔案不存在 → 第一次執行
    }
    if (fread(&counter, sizeof(int), 1, fp) != 1) {
        counter = 0;  // 讀取失敗 → 從 0 開始
    }
    fclose(fp);
    return counter;
}

// 寫回執行次數
void set_counter(int counter) {
    FILE *fp = fopen(COUNTER_FILE, "wb");
    if (fp == NULL) {
        perror("無法寫入 counter.dat");
        return;
    }
    if (fwrite(&counter, sizeof(int), 1, fp) != 1) {
        perror("寫入 counter 失敗");
    }
    fclose(fp);
}

// 檢查數字是否已存在於陣列
int num_in_numset(int num, int numset[]) {
    for (int i = 0; i < LOTTO_COUNT; i++) {
        if (numset[i] == num) return 1;
    }
    return 0;
}

// 氣泡排序（由小到大）
void bubble_sort(int numset[]) {
    for (int i = 0; i < LOTTO_COUNT - 1; i++) {
        for (int j = 0; j < LOTTO_COUNT - i - 1; j++) {
            if (numset[j] > numset[j + 1]) {
                int temp = numset[j];
                numset[j] = numset[j + 1];
                numset[j + 1] = temp;
            }
        }
    }
}

// 輸出排序後的一列號碼
void output_lottorow(FILE *fp, int numset[]) {
    bubble_sort(numset);
    for (int i = 0; i < LOTTO_COUNT; i++) {
        fprintf(fp, "%02d ", numset[i]);
    }
    fprintf(fp, "\n");
}

// 主要輸出函數：寫入 lotto.txt
void print_lottofile(int num_set, int counter, const char *lotto_file) {
    FILE *tmpfp = fopen(lotto_file, "w");
    if (tmpfp == NULL) {
        perror("無法開啟 lotto.txt");
        return;
    }

    time_t curtime;
    time(&curtime);
    char *time_str = ctime(&curtime);
    time_str[strcspn(time_str, "\n")] = '\0';  // 移除換行

    fprintf(tmpfp, "======== lotto649 ========\n");
    fprintf(tmpfp, "No. %05d\n", counter + 1);
    fprintf(tmpfp, "%s\n", time_str);

    // 產生 num_set 組號碼
    for (int set = 0; set < num_set; set++) {
        int numset[LOTTO_COUNT] = {0};

        // 產生 6 個不重複號碼
        for (int i = 0; i < LOTTO_COUNT; i++) {
            int num;
            do {
                num = (rand() % LOTTO_RANGE) + 1;
            } while (num_in_numset(num, numset));
            numset[i] = num;
        }

        fprintf(tmpfp, "[%d]: ", set + 1);
        output_lottorow(tmpfp, numset);
    }

    // 補滿至 5 組（若 num_set < 5）
    for (int set = num_set; set < 5; set++) {
        fprintf(tmpfp, "[%d]: -- -- -- -- -- --\n", set + 1);
    }

    fprintf(tmpfp, "======== cs2400 ========");
    fclose(tmpfp);
}
