/**
 * This file will be the database for the directory. I , by myself,
 * like an idiot, will manually create the query instructions and
 * how to handle the logic of this stuff. This will not only define
 * the structs but also read from and write to files to be used
 * within the employee site.
 *
 * @author Ja'Quis Franklin, Developer at MayDayz Smokn BBQ
 * @date 10/07/2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "employee.h"

#define MAX_EMPLOYEES 200
#define PASSWORD_BUFFER 188

int main(void)
{

    // Used in demonstration to see how long the program takes to order through loops
    clock_t start, end;

    double time_used;

    start = clock();

    // Creating a File to write the final piece of information to
    FILE *maydayz_emp_inforamtion;
    maydayz_emp_inforamtion = fopen("secrued_info.json", "w");
    if (maydayz_emp_inforamtion == NULL)
    {
        perror("Error Opening text file for MayDayz Employees");
        exit(EXIT_FAILURE);
    }

    // Testing functions
    struct Employee *employees = (struct Employee *)calloc(MAX_EMPLOYEES, sizeof(struct Employee));
    if (employees == NULL)
    {
        perror("Failed to allocate memory for employees.\n");
        return 1;
    }
    else
    {
        printf("Allocation succesful\n");
    }

    struct Employee employee1 = createEmployee("Ja'Quis", "Avshia", "Franklin", "704-967-5408", "jaquis.franklin@maydayz.com", "6500 Monteith Drive",
                                               "M-3455663", "Avashia100604!!", "10/06/04", "06/01/24", "Admin", "CTO", 334844843, 01234567, 56.673445, "000-01-9989");
    struct Employee employee2 = createEmployee("Lamar", "Jevon", "Henderson", "213-555-8192", "lamar.henderson@soxietytech.com", "8429 Sunset Blvd, Apt 12C",
                                               "HndrsonL001", "P@ssW0rd!93", "11/22/1993", "07/15/2022",
                                               "Engineer", "Backend Developer", 547829103, 10293847, 50.75, "000-47-8392");
    struct Employee employee3 = createEmployee("Lamar", "Jacob", "Green", "213-555-8192", "lamar.green@soxietytech.com", "9248 Sunrise Blvd, Apt 12A", "GreenL001",
                                               "P@ssW0rd!94", "11/22/2003", "07/15/2025",
                                               "Engineer", "Backend Developer", 547829103, 10293847, 50.75, "000-47-8392");

    *employees = employee1;
    *(employees + 1) = employee2;
    *(employees + 2) = employee3;

    size_t passcode_array_size = sizeof(employee1.passcode);

    char *passwords = (char *)calloc(MAX_EMPLOYEES, PASSWORD_BUFFER);
    if (passwords == NULL)
    {
        perror("Failed to allocate raw password buffer");
        free(employees);
        return 1;
    }
    displayEmployees(employees, 10);
    searchEmployeeByName(employees);
    searchEmployeeByUsername(employees);
    for (int i = 0; i < 3; i++)
    {
        char *current_raw_password = passwords + (i * passcode_array_size);
        int password_hash = hashPasscode(current_raw_password, employees[i].passcode, PASSWORD_BUFFER);

        printf("%s Hashed Password: %d, %s\n", employees[i].first_name, password_hash, employees[i].passcode);
    }

    displayEmployees(employees, 10);
    printf("\n");
    fprintf(maydayz_emp_inforamtion, "{ \n");
    fprintf(maydayz_emp_inforamtion, "  \"employees\": [\n");
    for (int i = 0; i < 3; i++)
    {
        fprintf(maydayz_emp_inforamtion,
                "    {\n"
                "      \"name\": \"%s %s %s\",\n"
                "      \"dob\": \"%s\",\n"
                "      \"address\": \"%s\",\n"
                "      \"role\": \"%s\",\n"
                "      \"position\": \"%s\",\n"
                "      \"date_hired\": \"%s\"\n"
                "    }%s\n",
                employees[i].first_name, employees[i].middle_name, employees[i].last_name, employees[i].birthday,
                employees[i].mailing_address, employees[i].role, employees[i].position, employees[i].hire_date,
                // Added a ternary operator to check if the object is last
                (i < 2) ? "," : "");
    }
    fprintf(maydayz_emp_inforamtion, "  ]\n");
    fprintf(maydayz_emp_inforamtion, "} ");
    printf("Successfully wrote to secured info file!\n");

    free(employees);
    free(passwords);

    end = clock();

    time_used = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Execution Time: %f seconds\n", time_used);
}
