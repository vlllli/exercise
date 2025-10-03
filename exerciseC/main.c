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

printf("�������");
scanf("%f",&new_record->amount);

while(getchar() !='\n' );

printf("���������");
fgets(new_record->category,50,stdin);

new_record->category[strcspn(new_record->category,"\n")]=0;

new_record->next = NULL;

    if (g_record_head == NULL) {
        // ��������ǿյģ��¼�¼����ͷ�ڵ�
        g_record_head = new_record;
    } else {
        // ���򣬱���������ĩβ��Ȼ�����
        FinancialRecord *current = g_record_head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_record;
    }

    g_record_count++;
    printf("��¼��ӳɹ���\n");
}

void display_all_records() {
    if (g_record_head == NULL) {
        printf("\n��ǰû���κμ�¼��\n");
        return;
    }

    printf("\n--- ���в����¼ ---\n");
    printf("����\t\t����\t���\t���\t\t��ע\n");
    printf("------------------------------------------------------------\n");

    FinancialRecord *current = g_record_head;
    while (current != NULL) {
        printf("%04d-%02d-%02d\t%s\t%.2f\t%s\t\t%s\n",
               current->year, current->month, current->day,
               (current->type == income) ? "����" : "֧��",
               current->amount,
               current->category,
               current->note);
        current = current->next;
    }
    printf("�ܹ� %d ����¼��\n", g_record_count);
}

void statistics_by_month() {
    if (g_record_head == NULL) {
        printf("\n��ǰû���κμ�¼��\n");
        return;
    }

    int input_year, input_month;
    printf("\n--- �¶�ͳ�� ---\n");
    printf("������Ҫͳ�Ƶ������ (�� ��)��");
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

    printf("\n%d��%02d��ͳ�ƽ����\n", input_year, input_month);
    printf("�����룺  %.2f\n", total_income);
    printf("��֧����  %.2f\n", total_expense);
    printf("�¶Ƚ��ࣺ%.2f\n", total_income - total_expense);
}

void save_to_file() {
    FILE *file = fopen("financial_data.dat", "wb"); // "wb" ��ʾ�Զ�����д��ģʽ��
    if (file == NULL) {
        printf("�޷����ļ����б��棡\n");
        return;
    }

    // �Ƚ���¼����д���ļ��������ȡ
    fwrite(&g_record_count, sizeof(int), 1, file);

    FinancialRecord *current = g_record_head;
    while (current != NULL) {
        // ��ÿһ����¼д���ļ�
        fwrite(current, sizeof(FinancialRecord), 1, file);
        current = current->next;
    }

    fclose(file);
    printf("�����ѳɹ����浽 financial_data.dat\n");
}

void load_from_file() {
    FILE *file = fopen("financial_data.dat", "rb"); // "rb" ��ʾ�Զ����ƶ�ȡģʽ��
    if (file == NULL) {
        printf("δ�ҵ��浵�ļ������ӿռ�¼��ʼ��\n");
        return;
    }

    // ��ȡ��¼����
    int count;
    fread(&count, sizeof(int), 1, file);

    for (int i = 0; i < count; i++) {
        FinancialRecord *new_record = (FinancialRecord*)malloc(sizeof(FinancialRecord));
        if (new_record == NULL) {
            printf("�ڴ����ʧ�ܣ��������ݲ�������\n");
            break;
        }
        fread(new_record, sizeof(FinancialRecord), 1, file);
        new_record->next = NULL;

        // ���¼�¼��ӵ�����
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
    printf("�ɹ����ļ������� %d ����¼��\n", count);
}

void clear_all_records() {
    char confirm;
    printf("\n!!! ���� !!!\n");
    printf("�˲�����ɾ�����м�¼���Ҳ��ɻָ���\n");
    printf("ȷ��������(y/N): ");
    scanf(" %c", &confirm); // ע�� %c ǰ��Ŀո���������֮ǰ�Ļ��з�

    if (confirm == 'y' || confirm == 'Y') {
        // �ͷ������е����нڵ㣬��ֹ�ڴ�й©
        FinancialRecord *current = g_record_head;
        FinancialRecord *next;
        while (current != NULL) {
            next = current->next;
            free(current);
            current = next;
        }
        // ����ȫ�ֱ���
        g_record_head = NULL;
        g_record_count = 0;

        // ��ѡ��ͬʱɾ�������ļ�
        if (remove("financial_data.dat") == 0) {
            printf("�����ļ���ɾ����\n");
        } else {
            // ����ļ������ڣ�ɾ����ʧ�ܣ�����������
            printf("�浵�ļ��������\n");
        }

        printf("���м�¼�ѳɹ����㣡\n");
    } else {
        printf("������ȡ����\n");
    }
}
int main() {
    int choice;

    // ��������ʱ���ȳ��Դ��ļ���������
    load_from_file();

    do {
        // ��ӡ���˵�
        printf("\n=== ���˲������ϵͳ ===\n");
        printf("1. ��Ӽ�¼\n");
        printf("2. �鿴���м�¼\n");
        printf("3. �¶�ͳ��\n");
        printf("4. ��������\n");
        printf("5. �������м�¼\n");
        printf("0. �˳�\n");
        printf("��ѡ�������");
        scanf("%d", &choice);

        // ������뻺����
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
                printf("��лʹ��!�ټ�^-^\n");
                // �˳�ǰ����ѯ���Ƿ񱣴�
                printf("�Ƿ����˳�ǰ�������ݣ�(y/n): ");
                char save_choice = getchar();
                if (save_choice == 'y' || save_choice == 'Y') {
                    save_to_file();
                }
                break;
            default:
                printf("��Ч��ѡ�����������룡\n");
        }
    } while (choice != 0);

    // �������ǰ���ͷ������ڴ棬��ֹ�ڴ�й©
    FinancialRecord *current = g_record_head;
    FinancialRecord *next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    return 0;
}