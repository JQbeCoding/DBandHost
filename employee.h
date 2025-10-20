/**
 * Property of Maydayz Smokn Bar-Be-Cue LLC.
 * Any unauthorized use of this program is against
 * the law and is NOT open source for free use.
 */

#ifndef EMPLOYEE_H
#define EMPLOYEE_H
#define MAX_EMPLOYEES 200
#define TIME_COST 3
#define MEMORY_COST 65536
#define PARALLELISM 1
#define HASH_LENGTH 100
#define SALT_LENGTH 16
#define ENCODED_PASSCODE_BUFFER_SIZE 188
#define MALLOC_NAME_SIZE 188

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// Argon API used for password and SSN hashing
#include "argon2.h"
// For random bytes
#include <sys/random.h>
#include </dev/urandom>
#include <unistd.h>
#include <fcntl.h>

struct Employee
{
    char *first_name;
    char *middle_name;
    char *last_name;
    char *phone_number;
    char *email;
    char *mailing_address;
    char *username;
    // Using a hashed passcode
    char *passcode;
    char *birthday;
    char *hire_date;
    char *role;
    char *position;
    int bank_account_number;
    int bank_routing_number;
    float hourly_pay_rate;
    // SSN will also be hashed
    char *ssn;
};

/**
 * Quick display of all elements within for all employees.
 * This function will show all information regarding employee,
 * information.
 */
void displayEmployee(struct Employee employee)
{
    printf("Name: %s %.1s %s\nUsername: %s\nDOB: %s\nAddress: %s\nRole: %s\nPosition: %s\nDate Hired: %s\n", employee.first_name, employee.middle_name, employee.last_name, employee.username, employee.birthday,
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
    char *name = (char *)malloc(sizeof(char) * MALLOC_NAME_SIZE);
    printf("Enter the first name of the employee: ");
    int flag = 0;
    scanf("%187s", name);
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
    char *id = (char *)malloc(sizeof(char) * MALLOC_NAME_SIZE);
    printf("Enter the username of the employee: ");
    int flag = 0;
    scanf("%187s", id);
    printf("The id you entered: %s\n", id);
    for (int i = 0; i < MAX_EMPLOYEES; i++)
    {
        if (employees[i].username != NULL)
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
struct Employee createEmployee(char *first_name, char *middle_name, char *last_name, char *phone_number, char *email, char *mailing_address, char *username,
                               char *passcode, char *birthday, char *hire_date, char *role, char *position, int bank_account_number, int bank_routing_number, float hourly_pay_rate,
                               char *ssn)
{
    struct Employee new_employee;

    new_employee.first_name = strdup(first_name);
    new_employee.middle_name = strdup(middle_name);
    new_employee.last_name = strdup(last_name);
    new_employee.phone_number = strdup(phone_number);
    new_employee.email = strdup(email);
    new_employee.mailing_address = strdup(mailing_address);

    new_employee.username = strdup(username);

    // This is here to pad for the new hashed password as
    // otherwise it will cause memory corruption
    new_employee.passcode = (char *)malloc(ENCODED_PASSCODE_BUFFER_SIZE);
    if (new_employee.passcode == NULL)
    {
        perror("malloc failed for passcode buffer");
        exit(EXIT_FAILURE);
    }
    snprintf(new_employee.passcode, ENCODED_PASSCODE_BUFFER_SIZE, "%s", passcode);

    new_employee.birthday = strdup(birthday);

    new_employee.hire_date = strdup(hire_date);
    new_employee.role = strdup(role);
    new_employee.position = strdup(position);

    new_employee.bank_account_number = bank_account_number;
    new_employee.bank_routing_number = bank_routing_number;
    new_employee.hourly_pay_rate = hourly_pay_rate;
    // The same logic for the password is applied to
    // the ssn
    new_employee.ssn = (char *)malloc(ENCODED_PASSCODE_BUFFER_SIZE);
    if (new_employee.ssn == NULL)
    {
        perror("malloc failed for passcode buffer");
        exit(EXIT_FAILURE);
    }
    snprintf(new_employee.ssn, ENCODED_PASSCODE_BUFFER_SIZE, "%s", ssn);

    return new_employee;
}

// Added by Copilot
/**
 * Free all heap-allocated fields inside a single Employee struct.
 * This does not free the struct itself (since Employee is returned by
 * value in this project); it only frees fields allocated on the heap.
 */
void freeEmployee(struct Employee *emp)
{
    if (emp == NULL)
        return;

    free(emp->first_name);
    free(emp->middle_name);
    free(emp->last_name);
    free(emp->phone_number);
    free(emp->email);
    free(emp->mailing_address);
    free(emp->username);
    free(emp->passcode);
    free(emp->birthday);
    free(emp->hire_date);
    free(emp->role);
    free(emp->position);
    free(emp->ssn);

    emp->first_name = NULL;
    emp->middle_name = NULL;
    emp->last_name = NULL;
    emp->phone_number = NULL;
    emp->email = NULL;
    emp->mailing_address = NULL;
    emp->username = NULL;
    emp->passcode = NULL;
    emp->birthday = NULL;
    emp->hire_date = NULL;
    emp->role = NULL;
    emp->position = NULL;
    emp->ssn = NULL;
}

/**
 * Function to actually free all of the employees within
 * the database22
 */
void freeEmployees(struct Employee *employees, int size)
{
    if (employees == NULL)
        return;

    for (int i = 0; i < size; i++)
    {
        if (employees[i].first_name == NULL)
            break;
        freeEmployee(&employees[i]);
    }
}

/**
 * @brief Hashes the passcode and initializes the password to an argon2 password.
 *
 * This function uses the raw password given from the user and converts it to a
 * hash value that is safer for a production database.
 * @param raw_password is a pointer to the password given from the user.
 * @param raw_password_buffer is the pointer to the address or location of the current
 * password
 * @param buffer_size is the allocated size in bytes for the new password
 */
int hashPasscode(char *raw_password, char *raw_password_buffer, size_t buffer_size)
{
    size_t password_size = strlen(raw_password);
    size_t required_size = argon2_encodedlen(TIME_COST, MEMORY_COST, PARALLELISM, SALT_LENGTH, HASH_LENGTH, Argon2_i);
    uint8_t salt[SALT_LENGTH];

    int urandom_fd = open("/dev/urandom", O_RDONLY);
    if (urandom_fd == -1)
    {
        fprintf(stderr, "Error: Could not open /dev/urandom\n");
        return -3;
    }
    ssize_t bytes_read = read(urandom_fd, salt, SALT_LENGTH);
    close(urandom_fd);
    if (bytes_read != SALT_LENGTH)
    {
        fprintf(stderr, "Error: Failed to read %d bytes from /dev/urandom. Read %zd.\n",
                SALT_LENGTH, bytes_read);
        return -4;
    }
    if (buffer_size < required_size)
    {
        fprintf(stderr, "Error: Hashed passcode buffer size (%zu) is too small. Required: %zu bytes.\n",
                buffer_size, required_size);
        return -2;
    }

    int hashedCode = argon2i_hash_encoded(TIME_COST, MEMORY_COST, PARALLELISM, raw_password, password_size, salt, SALT_LENGTH, HASH_LENGTH, raw_password_buffer, buffer_size);
    if (hashedCode != ARGON2_OK)
    {
        fprintf(stderr, "Argon2 hashing failed: %s\n", argon2_error_message(hashedCode));
    }
    return hashedCode;
}

void createUserName(struct Employee *emp)
{
    size_t min = 11111111;
    size_t max = 99999999;
    size_t serial_integer = rand() % (max - min + 1) + min;
    const size_t final_size = 2 + 8 + 1;
    char *new_username = (char *)malloc(final_size);
    if (new_username == NULL)
    {
        perror("Failed to allocate memory");
        return;
    }
    snprintf(new_username, final_size, "M-%zu", serial_integer);
    emp->username = new_username;
}

#endif
