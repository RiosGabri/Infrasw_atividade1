#include <stdio.h>
#include <string.h>

//int argc, char *argv[]
int main() {
    char linha[81];

    if (fgets(linha, sizeof(linha), stdin) != NULL) {
        printf("%s", linha);
    }

    return 0;
}