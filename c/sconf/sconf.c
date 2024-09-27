#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 1024
#define MAX_SECTION_LEN 256
#define MAX_VAR_LEN 256
#define MAX_VALUE_LEN 256

int main(int argc, char *argv[]) {
    if (argc < 3 || argc > 4) {
        printf("Usage: sui <get|set|delete> section.variable [value]\n");
        return 1;
    }

    char *command = argv[1];
    char *section_var = argv[2];
    char *value = argc == 4 ? argv[3] : NULL;

    // Parse section and variable names from argument
    char *section = strtok(section_var, ".");
    char *variable = strtok(NULL, ".");

    if (!section || !variable) {
        printf("Invalid section.variable format.\n");
        return 1;
    }

    // Open configuration file for reading/writing
    FILE *fp;
    char line[MAX_LINE_LEN];
    fp = fopen("config.cfg", "r+");
    if (!fp) {
        printf("Error opening configuration file.\n");
        return 1;
    }

    int section_found = 0;
    int section_created = 0;
    long section_start_pos = 0;
    long section_end_pos = 0;

    // Find section header and variable assignment
    while (fgets(line, sizeof(line), fp)) {
        // Check for section header
        if (line[0] == '[') {
            char tmp[MAX_SECTION_LEN];
            sscanf(line, "[%[^]]", tmp);
            if (strcmp(tmp, section) == 0) {
                section_found = 1;
                section_start_pos = ftell(fp) - strlen(line);
            } else if (section_found) {
                section_end_pos = ftell(fp) - strlen(line);
                break;
            }
        }
        // Check for variable assignment
        else if (section_found) {
            char tmp_var[MAX_VAR_LEN];
            char tmp_value[MAX_VALUE_LEN];
            sscanf(line, "%[^=] = %[^\n]", tmp_var, tmp_value);
            if (strcmp(tmp_var, variable) == 0) {
                // Perform command
                if (strcmp(command, "get") == 0) {
                    printf("%s\n", tmp_value);
                } else if (strcmp(command, "set") == 0) {
                    fseek(fp, -strlen(line), SEEK_CUR);
                    fprintf(fp, "%s = %s\n", variable, value);
                } else if (strcmp(command, "delete") == 0) {
                    fseek(fp, -strlen(line), SEEK_CUR);
                    fprintf(fp, "\n");
                }
                fclose(fp);
                return 0;
            }
        }
    }

    // Variable not found
    if (!section_found && strcmp(command, "set") == 0) {
        // Create new section header
        fseek(fp, 0, SEEK_END);
        fprintf(fp, "\n[%s]\n", section);
        section_created = 1;
    } else {
        printf("Variable %s.%s not found.\n", section, variable);
        fclose(fp);
        return 1;
    }

    // Add new variable assignment
    fseek(fp, section_created ? section_end_pos : section_start_pos, SEEK_SET);
    fprintf(fp, "%s = %s\n", variable, value);

    fclose(fp);
    return 0;
}

