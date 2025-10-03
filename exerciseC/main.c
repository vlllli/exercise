#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum 
{income=0,expense=1
} RecordType;

typedef struct FinancialRecord
{int year,month,day;
 RecordType type;
 float amount;
 char category[50];
 char note [100];
 struct FinancialRecord *next;
} FinancialRecord;
FinancialRecord *g_record_head=NULL;
int g_record_count=0;

void add_record()
{FinancialRecord *new_record=(FinancialRecord*)malloc(sizeof(FinancialRecord));
if (new_record==NULL) {
	printf("falt\n");
    return;
}

printf("\n---add new record---\n");
printf("add date:");
scanf("%d %d %d",&new_record->year,&new_record->month,&new_record->day);

printf("add your type(0-input,1-output):");
int type_input;
scanf("%d",&type_input);
new_record->type=(type_input==0) ? income:expense;

printf("请输入金额：");
scanf("%f",&new_record->amount);

while(getchar() !='\n' );

printf("请输入类别：");
fgets(new_record->category,50,stdin);

new_record->category[strcspn(new_record->category,"\n")]=0;

new_record->next = NULL;

    if (g_record_head == NULL) {
        // 如果链表是空的，新记录就是头节点
        g_record_head = new_record;
    } else {
        // 否则，遍历到链表末尾，然后添加
        FinancialRecord *current = g_record_head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_record;
    }

    g_record_count++;
    printf("记录添加成功！\n");
}

void display_all_records() {
    if (g_record_head == NULL) {
        printf("\n当前没有任何记录。\n");
        return;
    }

    printf("\n--- 所有财务记录 ---\n");
    printf("日期\t\t类型\t金额\t类别\t\t备注\n");
    printf("------------------------------------------------------------\n");

    FinancialRecord *current = g_record_head;
    while (current != NULL) {
        printf("%04d-%02d-%02d\t%s\t%.2f\t%s\t\t%s\n",
               current->year, current->month, current->day,
               (current->type == income) ? "收入" : "支出",
               current->amount,
               current->category,
               current->note);
        current = current->next;
    }
    printf("总共 %d 条记录。\n", g_record_count);
}

void statistics_by_month() {
    if (g_record_head == NULL) {
        printf("\n当前没有任何记录。\n");
        return;
    }

    int input_year, input_month;
    printf("\n--- 月度统计 ---\n");
    printf("请输入要统计的年和月 (年 月)：");
    scanf("%d %d", &input_year, &input_month);

    float total_income = 0.0f;
    float total_expense = 0.0f;

    FinancialRecord *current = g_record_head;
    while (current != NULL) {
        if (current->year == input_year && current->month == input_month) {
            if (current->type == income) {
                total_income += current->amount;
            } else {
                total_expense += current->amount;
            }
        }
        current = current->next;
    }

    printf("\n%d年%02d月统计结果：\n", input_year, input_month);
    printf("总收入：  %.2f\n", total_income);
    printf("总支出：  %.2f\n", total_expense);
    printf("月度结余：%.2f\n", total_income - total_expense);
}

void save_to_file() {
    FILE *file = fopen("financial_data.dat", "wb"); // "wb" 表示以二进制写入模式打开
    if (file == NULL) {
        printf("无法打开文件进行保存！\n");
        return;
    }

    // 先将记录总数写入文件，方便读取
    fwrite(&g_record_count, sizeof(int), 1, file);

    FinancialRecord *current = g_record_head;
    while (current != NULL) {
        // 将每一条记录写入文件
        fwrite(current, sizeof(FinancialRecord), 1, file);
        current = current->next;
    }

    fclose(file);
    printf("数据已成功保存到 financial_data.dat\n");
}

void load_from_file() {
    FILE *file = fopen("financial_data.dat", "rb"); // "rb" 表示以二进制读取模式打开
    if (file == NULL) {
        printf("未找到存档文件，将从空记录开始。\n");
        return;
    }

    // 读取记录总数
    int count;
    fread(&count, sizeof(int), 1, file);

    for (int i = 0; i < count; i++) {
        FinancialRecord *new_record = (FinancialRecord*)malloc(sizeof(FinancialRecord));
        if (new_record == NULL) {
            printf("内存分配失败，加载数据不完整！\n");
            break;
        }
        fread(new_record, sizeof(FinancialRecord), 1, file);
        new_record->next = NULL;

        // 将新记录添加到链表
        if (g_record_head == NULL) {
            g_record_head = new_record;
        } else {
            FinancialRecord *current = g_record_head;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = new_record;
        }
        g_record_count++;
    }

    fclose(file);
    printf("成功从文件加载了 %d 条记录。\n", count);
}

void clear_all_records() {
    char confirm;
    printf("\n!!! 警告 !!!\n");
    printf("此操作将删除所有记录，且不可恢复！\n");
    printf("确认清零吗？(y/N): ");
    scanf(" %c", &confirm); // 注意 %c 前面的空格，用于吸收之前的换行符

    if (confirm == 'y' || confirm == 'Y') {
        // 释放链表中的所有节点，防止内存泄漏
        FinancialRecord *current = g_record_head;
        FinancialRecord *next;
        while (current != NULL) {
            next = current->next;
            free(current);
            current = next;
        }
        // 重置全局变量
        g_record_head = NULL;
        g_record_count = 0;

        // 可选：同时删除数据文件
        if (remove("financial_data.dat") == 0) {
            printf("数据文件已删除。\n");
        } else {
            // 如果文件不存在，删除会失败，这是正常的
            printf("存档文件已清除。\n");
        }

        printf("所有记录已成功清零！\n");
    } else {
        printf("操作已取消。\n");
    }
}
int main() {
    int choice;

    // 程序启动时，先尝试从文件加载数据
    load_from_file();

    do {
        // 打印主菜单
        printf("\n=== 个人财务管理系统 ===\n");
        printf("1. 添加记录\n");
        printf("2. 查看所有记录\n");
        printf("3. 月度统计\n");
        printf("4. 保存数据\n");
        printf("5. 清零所有记录\n");
        printf("0. 退出\n");
        printf("请选择操作：");
        scanf("%d", &choice);

        // 清空输入缓冲区
        while (getchar() != '\n');

        switch (choice) {
            case 1:
                add_record();
                break;
            case 2:
                display_all_records();
                break;
            case 3:
                statistics_by_month();
                break;
            case 4:
                save_to_file();
                break;
            case 5:
            	clear_all_records();
            	break;
            case 0:
                printf("感谢使用!再见^-^\n");
                // 退出前可以询问是否保存
                printf("是否在退出前保存数据？(y/n): ");
                char save_choice = getchar();
                if (save_choice == 'y' || save_choice == 'Y') {
                    save_to_file();
                }
                break;
            default:
                printf("无效的选择，请重新输入！\n");
        }
    } while (choice != 0);

    // 程序结束前，释放链表内存，防止内存泄漏
    FinancialRecord *current = g_record_head;
    FinancialRecord *next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    return 0;
}