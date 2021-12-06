#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct uint_1024{
    unsigned char number[129];
    unsigned short count_zero : 7;
};

struct very_Big_Mas{
    unsigned long long int number;
    short zero_Count : 7;
};

struct uint_1024 from_uint(unsigned int int_num){
    struct uint_1024 element;
    element.count_zero = 0;
    short size = 0;
    unsigned int save_num = int_num;
    if (int_num == 0){
        element.number[0] = '\0';
        element.count_zero = 1;
        size = 1;
        return element;
    }
    else{
        while (save_num > 0){
            save_num = (int)(save_num / 256);
            size += 1;
        }
        for (short i = size - 1; i >= 0; i--){
            element.number[i] = (int_num % 256);
            if (int_num % 256 == 0)
                element.count_zero++;
            int_num = (int)(int_num / 256);
        }
    }
    element.number[size] = '\0';
    return element;
}

struct uint_1024 add_op(struct uint_1024 elem1, struct uint_1024 elem2){
    unsigned short size1 = 0;
    unsigned short size2 = 0;
    unsigned short size;
    short local_count_zero = elem1.count_zero;
    for (short i = 0; i <= 127; i++){
        if (elem1.number[i] == '\0' && local_count_zero == 0)
            break;
        else if(elem1.number[i] == '\0')
            local_count_zero--;
        size1++;
    }
    local_count_zero = elem2.count_zero;
    for (short i = 0; i <= 127; i++){
        if (elem2.number[i] == '\0' && local_count_zero == 0)
            break;
        else if(elem2.number[i] == '\0')
            local_count_zero--;
        size2++;
    }
    if (size1 > size2){
        size = size1;
        for (short i = size - 1; i >= size - size2; i--){
            elem2.number[i] = elem2.number[i - (size - size2)];
        }
        for (short i = 0; i <= size - size2 - 1; i++){
            elem2.number[i] = 0;
            elem2.count_zero++;
        }
    }
    else if(size1 < size2){
        size = size2;
        for (short i = size - 1; i >= size - size1; i--){
            elem1.number[i] = elem1.number[i - (size - size1)];
        }
        for (short i = 0; i <= size - size1 - 1; i++){
            elem1.number[i] = 0;
            elem1.count_zero++;
        }
    }
    else
        size = size1;
    short active_Num;
    elem1.count_zero = 0;
    unsigned short last_byte = 0;
    for (short byte = size - 1; byte >= 0; byte--){
        active_Num = (int)elem1.number[byte] + (int)elem2.number[byte] + last_byte;
        elem1.number[byte] = active_Num % 256;
        if (elem1.number[byte] == 0)
            elem1.count_zero++;
        last_byte = (short)(active_Num / 256);
    }
    if (last_byte > 0 && size >= 128)
        printf("Undefined Behavior\n");
    else if (last_byte > 0){
        for (short i = size; i >= 1; i--)
            elem1.number[i] = elem1.number[i - 1];
        elem1.number[0] = last_byte;
        elem1.number[size + 1] = '\0';
    }
    else
        elem1.number[size] = '\0';
    return elem1;
}

struct uint_1024 subtr_op(struct uint_1024 elem1, struct uint_1024 elem2){
    unsigned short size1 = 0;
    unsigned short size2 = 0;
    unsigned short size;
    short local_count_zero = elem1.count_zero;
    for (short i = 0; i <= 127; i++){
        if (elem1.number[i] == '\0' && local_count_zero == 0)
            break;
        else if(elem1.number[i] == '\0')
            local_count_zero--;
        size1++;
    }
    local_count_zero = elem2.count_zero;
    for (short i = 0; i <= 127; i++){
        if (elem2.number[i] == '\0' && local_count_zero == 0)
            break;
        else if(elem2.number[i] == '\0')
            local_count_zero--;
        size2++;
    }
    if (size1 > size2){
        size = size1;
        for (short i = size - 1; i >= size - size2; i--){
            elem2.number[i] = elem2.number[i - (size - size2)];
        }
        for (short i = 0; i <= size - size2 - 1; i++){
            elem2.number[i] = 0;
            elem2.count_zero++;
        }
    }
    else if(size1 < size2)
        printf("Undefined Behavior\n");
    else
        size = size1;
    elem1.count_zero = 0;
    short a;
    short b;
    short CF = 0;
    local_count_zero = 0;
    for (short byte = size - 1; byte >= 0; byte--){
        a = (int)elem1.number[byte] - CF;
        b = (int)elem2.number[byte];
        CF = 0;
        if (b > a && byte == 0)
            printf("Undefined Behavior\n");
        if (b > a){
            a += 256;
            CF = 1;
        }
        elem1.number[byte] = a - b;
        if (a - b == 0)
            local_count_zero++;
        else{
            elem1.count_zero += local_count_zero;
            local_count_zero = 0;
        }
    }
    for (short i = 0; i <= size - 1 - local_count_zero; i++)
        elem1.number[i] = elem1.number[i + local_count_zero];
    elem1.number[size - local_count_zero] = '\0';
    return elem1;
}

struct uint_1024 mult_op(struct uint_1024 elem1, struct uint_1024 elem2){
    struct uint_1024 total_elem;
    total_elem.number[0] = '\0';
    total_elem.count_zero = 0;
    unsigned short size1 = 0;
    unsigned short size2 = 0;
    unsigned short size_up;
    unsigned short size_down;
    unsigned short local_count_zero = elem1.count_zero;
    struct uint_1024 *up_elem;
    struct uint_1024 *down_elem;
    for (short i = 0; i <= 127; i++){
        if (elem1.number[i] == '\0' && local_count_zero == 0)
            break;
        else if(elem1.number[i] == '\0')
            local_count_zero--;
        size1++;
    }
    local_count_zero = elem2.count_zero;
    for (short i = 0; i <= 127; i++){
        if (elem2.number[i] == '\0' && local_count_zero == 0)
            break;
        else if(elem2.number[i] == '\0')
            local_count_zero--;
        size2++;
    }
    if (size2 < size1){
        size_up = size1;
        up_elem = &elem1;
        size_down = size2;
        down_elem = &elem2;
    }
    else{
        size_up = size2;
        up_elem = &elem2;
        size_down = size1;
        down_elem = &elem1;
    }
    unsigned short *last_Bytes_During_Multiply = (unsigned short*)calloc(size_down, sizeof(unsigned short));
    unsigned short *mas_Of_Active_Number = (unsigned short*)calloc(size_down, sizeof(unsigned short));
    int active_Multiply;
    unsigned short byte = 0;
    for (short i = size_up - 1; i >= 0; i--){
        for (short j = 0; j <= size_down - 1; j++){
            active_Multiply = up_elem->number[i] * down_elem->number[size_down - 1 - j];
            mas_Of_Active_Number[j] += active_Multiply % 256;
            last_Bytes_During_Multiply[j] = (int)(active_Multiply / 256) + (int)(mas_Of_Active_Number[j] / 256);
            mas_Of_Active_Number[j] = mas_Of_Active_Number[j] % 256;
        }
        if (mas_Of_Active_Number[0] == 0)
            total_elem.count_zero++;
        for (short k = byte; k >= 1; k--)
            total_elem.number[k] = total_elem.number[k - 1];
        if (byte >= 128)
            printf("Undefined Behavior\n");
        total_elem.number[0] = mas_Of_Active_Number[0];
        byte++;
        for (short k = 0; k <= size_down - 2; k++)
            mas_Of_Active_Number[k] = mas_Of_Active_Number[k + 1] + last_Bytes_During_Multiply[k];
        mas_Of_Active_Number[size_down - 1] = last_Bytes_During_Multiply[size_down - 1];
    }
    unsigned short CF = 0;
    for (short i = 0; i <= size_down - 1; i++){
        if (mas_Of_Active_Number[i] == 0 && i == size_down - 1)
            break;
        if (mas_Of_Active_Number[i] == 0)
            total_elem.count_zero++;
        for (short k = byte; k >= 1; k--)
            total_elem.number[k] = total_elem.number[k - 1];
        if (byte >= 128)
            printf("Undefined Behavior\n");
        if (mas_Of_Active_Number[i] >= 256){
            if (i <= size_down - 2)
                mas_Of_Active_Number[i + 1] += (int)(mas_Of_Active_Number[i] / 256);
            else if (i == size_down - 1)
                CF = (int)(mas_Of_Active_Number[i] / 256);
            mas_Of_Active_Number[i] = mas_Of_Active_Number[i] % 256;
        }
        total_elem.number[0] = mas_Of_Active_Number[i];
        byte++;
    }
    if (CF != 0){
        total_elem.number[byte] = CF;
        byte += 1;
        }
    total_elem.number[byte] = '\0';
    free(last_Bytes_During_Multiply);
    free(mas_Of_Active_Number);
    return total_elem;
}

unsigned long long int fast_pow(unsigned long long int multi, unsigned short n){
    unsigned long long int rez = 1;
    while (n != 0){
        if (n % 2 == 1)
            rez *= multi;
        multi *= multi;
        n = (int)(n / 2);
    }
    return rez;
}

void printf_value(struct uint_1024 element){
    unsigned short size = 0;
    for (short i = 0; i <= 127; i++){
        if (element.number[i] == '\0' && element.count_zero == 0)
            break;
        else if(element.number[i] == '\0')
            element.count_zero--;
        size++;
    }

    if (element.number[0] == 0 && size == 1)
        printf("0");
    struct very_Big_Mas *very_Big_Mas = (struct very_Big_Mas*)calloc(18, sizeof(struct very_Big_Mas));
    unsigned long long int *active_Pow = (unsigned long long int*)calloc(18, sizeof(unsigned long long int));

    short j;
    int last_Bytes_During_Multiply[3];
    int mas_Of_Active_Number[3];
    int active_Multiply;
    unsigned long long int *active_Fast_Pow_1 = (unsigned long long int*)malloc(sizeof(unsigned long long int));
    unsigned long long int *active_Fast_Pow_2 = (unsigned long long int*)malloc(sizeof(unsigned long long int));
    int nums[3];
    int count;
    int save;
    for (short i = size - 1; i >= 0; i--){
        if (i == size - 1)
            active_Pow[0] = 1;
        else{
            count = 0;
            j = 0;
            active_Multiply = 0;
            for (short L = 0; L <= 2; L++){
                last_Bytes_During_Multiply[L] = 0;
                mas_Of_Active_Number[L] = 0;
            }
            while(active_Pow[j] != 0 || mas_Of_Active_Number[0] != 0 || mas_Of_Active_Number[1] != 0 || mas_Of_Active_Number[0] != 0 || last_Bytes_During_Multiply[0] != 0 || last_Bytes_During_Multiply[1] != 0 || last_Bytes_During_Multiply[2] != 0){
                for (short k = 1; k <= 18; k++){
                    *active_Fast_Pow_1 = fast_pow(10, k);
                    *active_Fast_Pow_2 = fast_pow(10, k - 1);
                    //Loop for multiply on 6
                    active_Multiply = (unsigned long long int)(((active_Pow[j] % *active_Fast_Pow_1) / *active_Fast_Pow_2)) * 6;
                    mas_Of_Active_Number[0] += active_Multiply % 10;
                    last_Bytes_During_Multiply[0] = (unsigned long long int)(active_Multiply / 10) + (unsigned long long int)(mas_Of_Active_Number[0] / 10);
                    mas_Of_Active_Number[0] = mas_Of_Active_Number[0] % 10;
                    //Loop for multiply on 5
                    active_Multiply = (unsigned long long int)(((active_Pow[j] % *active_Fast_Pow_1) / *active_Fast_Pow_2)) * 5;
                    mas_Of_Active_Number[1] += active_Multiply % 10;
                    last_Bytes_During_Multiply[1] = (unsigned long long int)(active_Multiply / 10) + (unsigned long long int)(mas_Of_Active_Number[1] / 10);
                    mas_Of_Active_Number[1] = mas_Of_Active_Number[1] % 10;
                    //Loop for multiply on 2
                    active_Multiply = (unsigned long long int)(((active_Pow[j] % *active_Fast_Pow_1) / *active_Fast_Pow_2)) * 2;
                    mas_Of_Active_Number[2] += active_Multiply % 10;
                    last_Bytes_During_Multiply[2] = (unsigned long long int)(active_Multiply / 10) + (unsigned long long int)(mas_Of_Active_Number[2] / 10);
                    mas_Of_Active_Number[2] = mas_Of_Active_Number[2] % 10;
                    //Change number num*k
                    active_Pow[j] -= ((unsigned long long int)(((active_Pow[j] % *active_Fast_Pow_1) / *active_Fast_Pow_2) - (mas_Of_Active_Number[0])) * *active_Fast_Pow_2);
                    if (k == 18 && (unsigned long long int)(active_Pow[j] / fast_pow(10, 18)) >= 1){
                        active_Pow[j] -= fast_pow(10, 18);
                        last_Bytes_During_Multiply[0]++;
                    }
                    mas_Of_Active_Number[0] = mas_Of_Active_Number[1] + last_Bytes_During_Multiply[0];
                    mas_Of_Active_Number[1] = mas_Of_Active_Number[2] + last_Bytes_During_Multiply[1];
                    mas_Of_Active_Number[2] = last_Bytes_During_Multiply[2];
                    if (count++ >= (size - 1 - i) * 3 || (active_Pow[j + 1] == 0 && mas_Of_Active_Number[0] == 0 && mas_Of_Active_Number[1] == 0 && mas_Of_Active_Number[0] != 0 && last_Bytes_During_Multiply[0] == 0 && last_Bytes_During_Multiply[1] == 0 && last_Bytes_During_Multiply[2] == 0))
                        break;
                }
                j++;
            }
        }
        if (element.number[i] != 0){
            j = 0;
            active_Multiply = 0;
            for (short L = 0; L <= 2; L++){
                last_Bytes_During_Multiply[L] = 0;
                mas_Of_Active_Number[L] = 0;
                nums[L] = 0;
            }
            while(active_Pow[j] != 0 || mas_Of_Active_Number[0] != 0 || mas_Of_Active_Number[1] != 0 || mas_Of_Active_Number[0] != 0 || last_Bytes_During_Multiply[0] != 0 || last_Bytes_During_Multiply[1] != 0 || last_Bytes_During_Multiply[2] != 0){
                nums[0] = (int)element.number[i] % 10;
                nums[1] = (int)(((int)element.number[i]) / 10) % 10;
                nums[2] = (int)(((int)element.number[i]) / 100);
                for (short k = 1; k <= 18; k++){
                    *active_Fast_Pow_1 = fast_pow(10, k);
                    *active_Fast_Pow_2 = fast_pow(10, k - 1);
                    //Loop for multiply on num[0]
                    active_Multiply = (unsigned long long int)(((active_Pow[j] % *active_Fast_Pow_1) / *active_Fast_Pow_2)) * nums[0];
                    mas_Of_Active_Number[0] += active_Multiply % 10;
                    last_Bytes_During_Multiply[0] = (unsigned long long int)(active_Multiply / 10) + (unsigned long long int)(mas_Of_Active_Number[0] / 10);
                    mas_Of_Active_Number[0] = mas_Of_Active_Number[0] % 10;
                    //Loop for multiply on num[1]
                    active_Multiply = (unsigned long long int)(((active_Pow[j] % *active_Fast_Pow_1) / *active_Fast_Pow_2)) * nums[1];
                    mas_Of_Active_Number[1] += active_Multiply % 10;
                    last_Bytes_During_Multiply[1] = (unsigned long long int)(active_Multiply / 10) + (unsigned long long int)(mas_Of_Active_Number[1] / 10);
                    mas_Of_Active_Number[1] = mas_Of_Active_Number[1] % 10;
                    //Loop for multiply on num[2]
                    active_Multiply = (unsigned long long int)(((active_Pow[j] % *active_Fast_Pow_1) / *active_Fast_Pow_2)) * nums[2];
                    mas_Of_Active_Number[2] += active_Multiply % 10;
                    last_Bytes_During_Multiply[2] = (unsigned long long int)(active_Multiply / 10) + (unsigned long long int)(mas_Of_Active_Number[2] / 10);
                    mas_Of_Active_Number[2] = mas_Of_Active_Number[2] % 10;
                    //Change number num*k
                    very_Big_Mas[j].number += mas_Of_Active_Number[0] * *active_Fast_Pow_2;
                    if (((unsigned long long int)((very_Big_Mas[j].number / *active_Fast_Pow_2)) % 10) == 0 && i == 0)
                        very_Big_Mas[j].zero_Count++;
                    else
                        very_Big_Mas[j].zero_Count = 0;
                    if (k == 18 && (unsigned long long int)(very_Big_Mas[j].number / fast_pow(10, 18)) >= 1){
                        very_Big_Mas[j].number -= fast_pow(10, 18);
                        last_Bytes_During_Multiply[0]++;
                    }
                    mas_Of_Active_Number[0] = mas_Of_Active_Number[1] + last_Bytes_During_Multiply[0];
                    mas_Of_Active_Number[1] = mas_Of_Active_Number[2] + last_Bytes_During_Multiply[1];
                    mas_Of_Active_Number[2] = last_Bytes_During_Multiply[2];
                }
                save = j++;
            }
        }
    }
    very_Big_Mas[save].zero_Count = 0;
    short check = 1;
    for (short i = 17; i >= 0; i--){
        if (very_Big_Mas[i].number == 0 && check == 1)
            continue;
        check = 0;
        if (very_Big_Mas[i].number == 0){
            for (short j = 1; j <= 18; j++)
                printf("0");
            continue;
        }
        for (short j = 1; j <= very_Big_Mas[i].zero_Count; j++)
            printf("0");
        printf("%llu", (very_Big_Mas[i].number));
    }
    free(active_Pow);
    free(very_Big_Mas);
    free(active_Fast_Pow_1);
    free(active_Fast_Pow_2);
    printf("\n");
}

void fscanf_value(struct uint_1024 *element, FILE *fread){
    char *line = (char*)calloc(309, sizeof(char));
    element->count_zero = 0;
    unsigned short add_byte = 0;
    char letter;
    short CounT = 0;
    while ((letter = fgetc(fread)) != EOF){
        if (CounT >= 309)
            printf("Undefined Behavior\n");
        if (letter == '0' || letter == '1' || letter == '2' || letter == '3' || letter == '4' || letter == '5' || letter == '6' || letter == '7' || letter == '8' || letter == '9')
            line[add_byte++] = letter;
        else
            break;
        CounT++;
    }
    line[add_byte] = '\0';
    if (line[0] == '\0')
        printf("Undefined Behavior\n");
    if (line[0] == '0' && line[1] == '\0'){
        element->count_zero = 1;
        element->number[0] = 0;
        element->number[1] = '\0';
    }
    else{
        unsigned short i;
        unsigned short byte;
        unsigned short size = 0;
        unsigned short active_minuend;
        while (line[0] != '0'){
            i = 3;
            byte = 0;
            active_minuend = ((int)(line[0] - '0') * 100) + ((int)(line[1] - '0') * 10) + (int)(line[2] - '0');
            if ((line[1] == '\0') || (line[2] == '\0') || ((line[3] == '\0') && (active_minuend <= 255))){
                for (unsigned short j = size; j >= 1; j--)
                    element->number[j] = element->number[j - 1];
                if (size >= 128)
                    printf("Undefined Behavior\n");
                size++;
                if (line[1] == '\0')
                    active_minuend = (int)(line[0] - '0');
                else if (line[2] == '\0')
                    active_minuend = ((int)(line[0] - '0') * 10) + (int)(line[1] - '0');
                else
                    active_minuend = ((int)(line[0] - '0') * 100) + ((int)(line[1] - '0') * 10) + (int)(line[2] - '0');
                element->number[0] = active_minuend;
                if (active_minuend == 0)
                    element->count_zero++;
                break;
            }
            if (active_minuend <= 255){
                active_minuend *= 10;
                active_minuend += line[i++] - '0';
            }
            while (1 == 1){
                line[byte] = ((int)(active_minuend / 256)) + '0';
                active_minuend -= (line[byte++] - '0') * 256;
                if (line[i] == '\0'){
                    for (short j = size; j >= 1; j--)
                        element->number[j] = element->number[j - 1];
                    if (size >= 128)
                        printf("Undefined Behavior\n");
                    size++;
                    element->number[0] = active_minuend;
                    if (active_minuend == 0)
                        element->count_zero++;
                    break;
                }
                active_minuend *= 10;
                active_minuend += line[i++] - '0';
            }
            line[byte] = '\0';
        }
        element->number[size] = '\0';
    }
    free(line);
}

int main(){
    FILE *fread1, *fread2;
    fread1 = fopen("txt.txt", "r");
    fread2 = fopen("txt.txtt", "r");
    struct uint_1024 a;
    fscanf_value(&a, fread1);
    struct uint_1024 b;
    fscanf_value(&b, fread2);
    struct uint_1024 c;
    printf("a = ");
    printf_value(a);
    printf("b = ");
    printf_value(b);
    c = mult_op(a, b);
    printf_value(c);
    c = subtr_op(b, a);
    printf_value(c);
    c = add_op(a, b);
    printf_value(c);
    fclose(fread1);
    fclose(fread2);
    return 0;
}