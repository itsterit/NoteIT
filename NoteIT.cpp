#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "SearchDir.h"
using namespace std;

int main()
{
    SetConsoleCP(1251);             // настраиваем кодировку консоли (ввод)
    SetConsoleOutputCP(1251);       // (вывод)
    setlocale(LC_ALL, ".1251");     // настраиваем внутренние механизмы на эту кодировку

    string FilePatch[100];
    uint8_t FilePatchCounter = 0;
    FilePatch[0] = ".";

    string FileName; // = ".";

    string DirContent[100];
    int PatchAmount = 0;
    
    char SelPatchChar[5], *end;
    int SelPatch = 0;

    bool OpenFile = 0;

    while (true)
    {
        system("cls");

        /* подготовка пути */
        FileName = "\0";
        for (uint8_t Counter = 0; Counter <= FilePatchCounter; Counter++)
        {
            FileName = FileName + FilePatch[Counter];
        }
        printf("\033[1;37;44m %s \033[0m", FileName.c_str());
        printf("\n\r");

        if (OpenFile == 1)
        {
            FILE* fp;
            if ((fp = fopen(FileName.c_str(), "r")) == NULL)
            {
                printf("Не удалось открыть файл!");
            }
            else
            {
                /* открытие файла */
                char Buffer[256] = { 0 };
                for (uint32_t StrCounter = 0; (fgets(Buffer, sizeof(Buffer), fp)) != NULL; StrCounter++)
                {
                    /* просмотр заголовка */
                    if (Buffer[0] == (char)'#')
                    {
                        printf("%d) ", StrCounter);
                        for (uint32_t CharCounter = 0; CharCounter <= strlen(Buffer); CharCounter++)
                        {
                            printf("%c", Buffer[CharCounter + 1]);
                        }
                    }
                }
                fclose(fp);
            }
        }
        else
        {
            /* количество эллементов папки и выравнивание */
            PatchAmount = SerchDir(&DirContent[0], &FileName);
            char NumLen[3] = { 0 };
            sprintf(NumLen, "%d", PatchAmount);
            short MaxNumLen = strlen(NumLen);

            /* вывод содержимого */
            for (uint8_t PatchCounter = 0; PatchCounter < PatchAmount; PatchCounter++)
            {
                printf("%d)", PatchCounter);
                sprintf(NumLen, "%d", PatchCounter);
                for (uint8_t AlignmentCounter = ((MaxNumLen + 1) - strlen(NumLen)); AlignmentCounter; AlignmentCounter--)
                {
                    printf(" ");
                }
                printf("%s \n\r", DirContent[PatchCounter].c_str());
            }
        }

        printf("\n\r> ");
        scanf("%s", SelPatchChar);

        if (strtol(SelPatchChar, &end, 10))
        {
            SelPatch = strtol(SelPatchChar, &end, 10);
        }
        else
        {
            /* чтение команды -q */
            if (SelPatchChar[0] == (char)'q' || SelPatchChar[0] == (char)'Q')
            {
                FileName = "\0";
                FilePatchCounter = 0;
                OpenFile = 0;
                continue;
            }
            else
            {
                if (SelPatchChar[0] == (char)'e' || SelPatchChar[0] == (char)'E')
                {
                    return 0;
                }
                else
                {
                    /* покидаем папку или файл без расширения */
                    continue;
                }
            }
        }

        /* проверка на максимальное число */
        if ((PatchAmount > SelPatch) && (SelPatch >= 0))
        {
            /* запрет на редактирование пути, если открыт файл */
            if (OpenFile == 0)
            {
                /* переход или открытие */
                short FolderOrFile = DirContent[SelPatch].find(".");
                if (FolderOrFile == 0)
                {
                    /* это комманда */
                    FolderOrFile = DirContent[SelPatch].find("..");
                    if (FolderOrFile == 0)
                    {
                        /* назад в директорию */
                        if (FilePatchCounter)
                        {
                            FilePatchCounter--;
                        }
                    }
                    else
                    {
                        /* остаёмся в директории */
                    }
                }
                else
                {
                    if (FolderOrFile < 0)
                    {
                        /* это папка */
                        FilePatchCounter++;
                        FilePatch[FilePatchCounter] = "\\" + DirContent[SelPatch];
                    }
                    else
                    {
                        /* это файл */
                        OpenFile = 1;
                        FilePatchCounter++;
                        FilePatch[FilePatchCounter] = "\\" + DirContent[SelPatch];
                    }
                }
            }
        }
    }


    scanf("%d", &PatchAmount);
}