#ifndef EMPLOYEE_H
#define EMPLOYEE_H
#define USERNAME_LENGTH 11
#define PASSWORD_LENGTH 16
#define BIRTHDAY_LENGTH 11
#define SSN_LENGTH 12
#define MAX_EMPLOYEES 200

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Employee
{
    char *first_name;
    char *middle_name;
    char *last_name;
    char *phone_number;
    char *email;
    char *mailing_address;
    char username[USERNAME_LENGTH];
    // Using a hashed passcode
    char passcode[PASSWORD_LENGTH];
    char birthday[BIRTHDAY_LENGTH];
    char *hire_date;
    char *role;
    char *position;
    int bank_account_number;
    int bank_routing_number;
    float hourly_pay_rate;
    // SSN will also be hashed
    char ssn[SSN_LENGTH];
};

/**
 * Quick display of all elements within for all employees.
 * This function will show all information regarding employee,
 * information.
 */
void displayEmployee(struct Employee employee)
{
    printf("Name: %s %.1s %s\nDOB: %s\nAddress: %s\nRole: %s\nPosition: %s\nDate Hired: %s\n", employee.first_name, employee.middle_name, employee.last_name, employee.birthday,
           employee.mailing_address, employee.role, employee.position, employee.hire_date);
}

/**
 * Quick display of all elements within the employees database.
 * This function will show all information regarding employee,
 * information.
 */
void displayEmployees(struct Employee *employees, int size)
{
    printf("\t\t\t\t\t–––––EMPLOYEES LIST–––––\t\t\t\t\t\n");
    for (int i = 0; i < size; i++)
    {
        if (employees[i].first_name == NULL)
        {
            break;
        }
        printf("[%d] ", i + 1);

        displayEmployee(employees[i]);
        printf("\n");
    }
    printf("\t\t\t\t\t––––––––––––––––––––––––\t\t\t\t\t\n");
}

/**
 * @brief Searches for an employee by their first name.
 *
 * This function prompts the user to enter the first name of an employee to search for.
 * It then iterates through the array of employees, performing a case-insensitive
 * comparison between the input name and each employee's first name.
 * If a match is found, it prints a confirmation message, displays the full
 * details of the employee using the `displayEmployee` function, and stops searching.
 * If no match is found after checking the entire array, it informs the user that
 * the employee could not be found.
 *
 * @param employees A pointer to the array of `struct Employee` that will be searched.
 * @note The function allocates and frees memory internally for the user's input.
 *       The search is case-insensitive.
 */
void searchEmployeeByName(struct Employee *employees)
{
    char *name = (char *)malloc(sizeof(char) * 20);
    printf("Enter the first name of the employee: ");
    int flag = 0;
    scanf("%s", name);
    for (int i = 0; i < MAX_EMPLOYEES; i++)
    {
        if (employees[i].first_name != NULL)
        {
            if (strcasecmp(employees[i].first_name, name) == 0)
            {
                displayEmployee(employees[i]);
                printf("\n");
                flag = 1;
            }
        }
    }
    if (!flag)
    {
        printf("Couldn't find the employee you were searching for.\n");
    }
    free(name);
}

void searchEmployeeByUsername(struct Employee *employees)
{
    char *id = (char *)malloc(sizeof(char) * 20);
    printf("Enter the username of the employee: ");
    int flag = 0;
    scanf("%s", id);
    for (int i = 0; i < MAX_EMPLOYEES; i++)
    {
        if (employees[i].first_name != NULL)
        {
            if (strcasecmp(employees[i].username, id) == 0)
            {
                displayEmployee(employees[i]);
                printf("\n");
                flag = 1;
            }
        }
    }
    if (!flag)
    {
        printf("Couldn't find the employee you were searching for.\n");
    }
    free(id);
}

/**
 * Creates a new Employee struct by prompting the user for input.
 * This function will ask the user to enter details for each field of the Employee struct.
 *
 * @param first_name Pointer to the first name string.
 * @param middle_name Pointer to the middle name string.
 * @param last_name Pointer to the last name string.
 * @param phone_number Pointer to the phone number string.
 * @param email Pointer to the email string.
 * @param mailing_address Pointer to the mailing address string.
 * @param username Array to store the username.
 * @param passcode Array to store the hashed passcode.
 * @param birthday Array to store the birthday.
 * @param hire_date Pointer to the hire date string.
 * @param role Pointer to the role string.
 * @param position Pointer to the position string.
 * @param bank_account_number Bank account number.
 * @param bank_routing_number Bank routing number.
 * @param hourly_pay_rate Hourly pay rate.
 * @param ssn Array to store the hashed SSN.
 * @return A struct Employee with the entered details.
 */
struct Employee createEmployee(char *first_name, char *middle_name, char *last_name, char *phone_number, char *email, char *mailing_address, char username[USERNAME_LENGTH],
                               char passcode[PASSWORD_LENGTH], char birthday[BIRTHDAY_LENGTH], char *hire_date, char *role, char *position, int bank_account_number, int bank_routing_number, float hourly_pay_rate,
                               char ssn[SSN_LENGTH])
{
    struct Employee new_employee;

    new_employee.first_name = strdup(first_name);
    new_employee.middle_name = strdup(middle_name);
    new_employee.last_name = strdup(last_name);
    new_employee.phone_number = strdup(phone_number);
    new_employee.email = strdup(email);
    new_employee.mailing_address = strdup(mailing_address);

    snprintf(new_employee.username, USERNAME_LENGTH, "%s", username);
    snprintf(new_employee.passcode, PASSWORD_LENGTH, "%s", passcode);
    snprintf(new_employee.birthday, BIRTHDAY_LENGTH, "%s", birthday);

    new_employee.hire_date = strdup(hire_date);
    new_employee.role = strdup(role);
    new_employee.position = strdup(position);

    new_employee.bank_account_number = bank_account_number;
    new_employee.bank_routing_number = bank_routing_number;
    new_employee.hourly_pay_rate = hourly_pay_rate;

    snprintf(new_employee.ssn, SSN_LENGTH, "%s", ssn);

    return new_employee;
}

#endif
