// Ofir Bitron 200042414

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

bool validateInput(int numOfArgs, char *args[])
/**
 * Validates user input
 * @param numOfArgs: number of args given by the user
 * @param args: All the parameters given by the user
 * @return: True if the input is valid else false
 */
{

    char *fileExtensionStr = ".\0";

    // OS types
    char *macStr = "-mac\0";
    char *windowsStr = "-win\0";
    char *unixStr = "-unix\0";

    int sourcePath = 1;
    int destPath = 2;
    int sourceOS = 3;
    int destOS = 4;
    int endianessFlag = 5;

    // Checks if source and dest files were given by user
    if (!strstr(args[sourcePath], fileExtensionStr) ||
            !strstr(args[destPath], fileExtensionStr))
    {
        return false;
    }

    FILE *fHandle = fopen(args[sourcePath], "r");

    // Check if source file exists on computer
    if (!fHandle)
    {
        return false;
    } else {
        fclose(fHandle);
    }

    // If the 3 or 4 flag arent OS flags
    if (numOfArgs >= 3)
    {

        if (!((strcmp(args[sourceOS], unixStr) == 0 ||
                strcmp(args[sourceOS], windowsStr) == 0 ||
                strcmp(args[sourceOS], macStr) == 0) &&
                (strcmp(args[destOS], unixStr) == 0 ||
                        strcmp(args[destOS], windowsStr) == 0 ||
                        strcmp(args[destOS], macStr) == 0)))
        {
            return false;
        }
    }

    return true;
}

bool copySourceToDest(char *sourcePath, char *destPath)
/**
 * Copies the source file to the dest file as is
 * @param sourcePath: The source file to copy
 * @param destPath: The destination file to be copied to
 * @return: True if copied else false
 */
{

    int windowsEOLone = 0x000d;
    int windowsEOLtwo = 0x000a;
    int unixEOL = 0x000a;
    int macEOL = 0x000d;

    size_t numOfBytesToRead = 2;
    char byteArray[2];

    FILE *srcHandle;
    FILE *dstHandle;

    srcHandle = fopen(sourcePath, "rb");

    // Checks if handle was opened to source file
    if (!srcHandle)
    {
        return false;
    }

    dstHandle = fopen(destPath, "wb");

    // Checks if handle was opened to dest file
    if (!dstHandle)
    {
        return false;
    }

    // Copy 2 bytes until end of file reached
    while (fread(byteArray, sizeof(char), numOfBytesToRead, srcHandle) != 0)
    {

        int i;

        // Print each byte seperately to the dest file
        for (i = 0; i < 2; i++)
        {
            fwrite(&byteArray[i], sizeof(char), 1, dstHandle);
        }
    }

    // Close handle to the files
    fclose(srcHandle);
    fclose(dstHandle);
    return true;
}

bool copyToNewOS(char *srcFile, char *dstFile, char *srcOS, char *dstOS, char *endianessFlag)
/**
 * Copies source file to destination file according to OS types
 * and according to endianess type
 * @param srcFile: Name of the source file
 * @param dstFile: Name of the destination file
 * @param srcOS: Flag of the source OS
 * @param dstOS: Flag of the destination OS
 * @param endianessFlag: Flag with endianess type
 * @return: True if succeede copying else false
 */
{

    // Handle to the source and dest file
    FILE *srcHandle;
    FILE *dstHandle;

    // Params for fopen function
    size_t numOfBytesToRead = 2;
    char byteArray[2];

    // OS flags
    char *windows = "-win\0";
    char *unixs = "-unix\0";
    char *mac = "-mac\0";

    // Endianess flags
    char *swapFlag = "-swap\0";
    char *keepFlag = "-keep\0";

    // OS end of line representation
    short windowsEOLone = 0x000d;
    short windowsEOLtwo = 0x000a;
    short unixEOL = 0x000a;
    short macEOL = 0x000d;

    // Stores byte representation of selected OS
    short srcOSType[2];
    short dstOSType[2];

    srcHandle = fopen(srcFile, "rb");

    // Checks if handle was opened to source file
    if (!srcHandle)
    {
        return false;
    }

    dstHandle = fopen(dstFile, "wb");

    // Checks if handle was opened to destination file
    if (!dstHandle)
    {
        return false;
    }

    // Put into srcOSType the relevant representation of EOL
    if (0 == strcmp(windows, srcOS))
    {
        srcOSType[0] = windowsEOLone;
        srcOSType[1] = windowsEOLtwo;
    } else if (0 == strcmp(unixs, srcOS)) {
        srcOSType[0] = unixEOL;
        srcOSType[1] = 0;
    } else {
        srcOSType[0] = macEOL;
        srcOSType[1] = 0;
    }

    // Put into dstOSType the relevant representation of EOL
    if (0 == strcmp(windows, dstOS))
    {
        dstOSType[0] = windowsEOLone;
        dstOSType[1] = windowsEOLtwo;
    } else if (0 == strcmp(unixs, dstOS)) {
        dstOSType[0] = unixEOL;
        dstOSType[1] = NULL;
    } else {
        dstOSType[0] = macEOL;
        dstOSType[1] = NULL;
    }

    // If swap flag was selected write bytes to dest file backwards
    if (!strcmp(endianessFlag, swapFlag))
    {

        // Copy 2 bytes until end of file reached
        while (fread(byteArray, sizeof(char), numOfBytesToRead, srcHandle) != 0)
        {

            // If end of line reached write dst EOL representation
            if (byteArray[0] == srcOSType[0])
            {

                // Insert the dest OS EOL representation instead
                fwrite(&dstOSType[1], sizeof(char), 1, dstHandle);
                fwrite(&dstOSType[0], sizeof(char), 1, dstHandle);
            } else if (byteArray[0] == srcOSType[1] && !strcmp(srcOS, windows)){

                continue;
            } else if(byteArray[0] == dstOSType[1] && !strcmp(dstOS, windows)) {

                fwrite(&dstOS[1], sizeof(char), 1, dstHandle);
                fwrite(&dstOS[0], sizeof(char), 1, dstHandle);
            }
            {

                // Copy the bytes backwards to the dest file
                fwrite(&byteArray[1], sizeof(char), 1, dstHandle);
                fwrite(&byteArray[0], sizeof(char), 1, dstHandle);
            }
        }
    } else{

        // Copy 2 bytes until end of file reached
        while (fread(byteArray, sizeof(char), numOfBytesToRead, srcHandle) != 0)
        {

            // If end of line reached write dst EOL representation
            if (byteArray[0] == srcOSType[0])
            {

                // Insert the dest OS EOL representation instead
                fwrite(&dstOSType[0], sizeof(char), 1, dstHandle);
                fwrite(&dstOSType[1], sizeof(char), 1, dstHandle);
            } else if (byteArray[0] == srcOSType[1] && !strcmp(srcOS, windows)){

                continue;
            } else if(byteArray[0] == dstOSType[1] && !strcmp(dstOS, windows)) {

                fwrite(&dstOS[0], sizeof(char), 1, dstHandle);
                fwrite(&dstOS[1], sizeof(char), 1, dstHandle);
            }
            else{

                // Copy the bytes as is to the dest file
                fwrite(&byteArray[0], sizeof(char), 1, dstHandle);
                fwrite(&byteArray[1], sizeof(char), 1, dstHandle);
            }
        }
    }

    // Close handle to the files
    fclose(srcHandle);
    fclose(dstHandle);
    return true;
}

int main(int argc, char *argv[]) {

    // If no destination path or dst OS type given
    // Or more than 5 params given
    if (argc < 3 || argc == 4 || argc > 6)
    {
        return 1;
    }

    // If input is invalid
    if (!validateInput(argc - 1, argv))
    {
        return 1;
    }

    // If only source and dest files given
    if (argc == 3)
    {
        copySourceToDest(argv[1], argv[2]);
    }

    // If source and dest file and OS given
    else if (argc <= 5)
    {
        copyToNewOS(argv[1], argv[2], argv[3], argv[4], "None");
    }

    // If source and dest file and OS and endianess flag given
    else if (argc == 6)
    {
        copyToNewOS(argv[1], argv[2], argv[3], argv[4], argv[5]);
    }

    return 0;
}