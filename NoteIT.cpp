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
    bool PrintItem = 0;

    short MaxNumLen = 0;

    while (true)
    {
        system("cls");

        /* подготовка пути */
        FileName = "\0";
        for (uint8_t Counter = 0; Counter <= FilePatchCounter; Counter++)
        {
            FileName = FileName + FilePatch[Counter];
        }
        printf("\033[1;37;44m %s \033[0m\n\n\r", FileName.c_str());

        /* просмотр файла */
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
                char NumLen[3] = { 0 };
                MaxNumLen = 0;

                for (uint32_t StrCounter = 0; (fgets(Buffer, sizeof(Buffer), fp)) != NULL; StrCounter++)
                {
                    sprintf(NumLen, "%d", StrCounter);
                    if (MaxNumLen < strlen(NumLen))
                    {
                        MaxNumLen = strlen(NumLen);
                    }
                }

                fp = fopen(FileName.c_str(), "r");
                memset(Buffer, 0, sizeof(Buffer));

                for (uint32_t StrCounter = 0, HeaderCounter = 0; (fgets(Buffer, sizeof(Buffer), fp)) != NULL; StrCounter++)
                {
                    /* просмотр заголовка */
                    if (Buffer[0] == (char)'#')
                    {
                        PrintItem = 0;
                        HeaderCounter++;

                        if (SelPatch == HeaderCounter || !SelPatch)
                        {
                            PrintItem = 1;

                            if (!SelPatch)
                            {
                                /* выравнивание */
                                printf("%d)", HeaderCounter);
                                sprintf(NumLen, "%d", HeaderCounter);
                                for (uint8_t AlignmentCounter = ((MaxNumLen + 1) - strlen(NumLen)); AlignmentCounter; AlignmentCounter--)
                                {
                                    printf(" ");
                                }
                            }

                            /* заголовок */
                            for (uint32_t CharCounter = 1; CharCounter <= strlen(Buffer); CharCounter++)
                            {
                                printf("%c", Buffer[CharCounter]);
                            }
                        }
                    }
                    else
                    {
                        if (PrintItem && SelPatch)
                        {
                            /* содержимое заголовка */
                            printf("%s", Buffer);
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
            MaxNumLen = strlen(NumLen);

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

        printf("> ");
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
                    if (SelPatchChar[0] == (char)'b' || SelPatchChar[0] == (char)'B')
                    {
                        SelPatch = 0;
                    }
                    else
                    {   
                        // printf("e - exit \n\r");
                        // printf("b - break \n\r");
                        // printf("q - exit \n\r");
                        continue;
                    }
                }
            }
        }

        /* запрет на редактирование пути, если открыт файл */
        if (OpenFile == 0)
        {
            /* проверка на максимальное число */
            if ((PatchAmount > SelPatch) && (SelPatch >= 0))
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
                         SelPatch = 0;
                     }
                 }
            }
        }
    }


    return 0;
}