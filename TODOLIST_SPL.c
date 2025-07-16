#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TASKS 100

struct Task {
    int serial_no;
    char title[100];
    char description[100];
    int completed;
    int year,month;
    int day,hour,minute;

};

void save_tasks(struct Task tasks[], int count) {
    FILE *file = fopen("tasks.txt", "w");
    if (file == NULL) {
        printf("Error opening file for writing!\n");
        return 0;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%d;%s;%s;%d;%d;%d;%d;%d;%d\n", tasks[i].serial_no, tasks[i].title, tasks[i].description, tasks[i].completed, tasks[i].year, tasks[i].month, tasks[i].day, tasks[i].hour, tasks[i].minute);
    }

    fclose(file);
}

int load_tasks(struct Task tasks[]) {
    FILE *file = fopen("tasks.txt", "r");
    if (file == NULL) {
        return 0;
    }

    int count = 0;
    while (fscanf(file, "%d;%49[^;];%99[^;];%d;%d;%d;%d;%d;%d\n", &tasks[count].serial_no, tasks[count].title, tasks[count].description, &tasks[count].completed, &tasks[count].year, &tasks[count].month, &tasks[count].day, &tasks[count].hour, &tasks[count].minute) == 9) {
        count++;
    }

    fclose(file);
    return count;
}

void print_task(struct Task task) {
    printf("\tSerial No: %d\n", task.serial_no);
    printf("\tTitle: %s\n", task.title);
    printf("\tDescription: %s\n", task.description);
    printf("\tDate: %d-%02d-%02d\n", task.year, task.month, task.day);
    printf("\tTime: %02d:%02d\n", task.hour, task.minute);
    printf("\tCompleted: %s\n", task.completed ? "Yes" : "No");
    printf("\t------------------------------------\n");
}

void create_task(struct Task tasks[], int *count) {
    if (*count >= MAX_TASKS) {
        printf("\tTask limit reached. Cannot add more tasks.\n");
        return;
    }

    struct Task new_task;
    printf("\tEnter title: ");
    getchar();
    fgets(new_task.title, sizeof(new_task.title), stdin);
    new_task.title[strcspn(new_task.title, "\n")] = '\0';

    printf("\tEnter description: ");
    fgets(new_task.description, sizeof(new_task.description), stdin);
    new_task.description[strcspn(new_task.description, "\n")] = '\0';

    printf("\tEnter year: ");
    scanf("%d", &new_task.year);
    printf("\tEnter month: ");
    scanf("%d", &new_task.month);
    printf("\tEnter day: ");
    scanf("%d", &new_task.day);
    printf("\tEnter hour: ");
    scanf("%d", &new_task.hour);
    printf("\tEnter minute: ");
    scanf("%d", &new_task.minute);

    new_task.serial_no = *count + 1;
    new_task.completed = 0;

    tasks[*count] = new_task;
    (*count)++;

    save_tasks(tasks, *count);
}

void mark_completed(struct Task tasks[], int count, int serial_no) {
    for (int i = 0; i < count; i++) {
        if (tasks[i].serial_no == serial_no) {
            tasks[i].completed = 1;
            printf("Task marked as completed.\n");
            save_tasks(tasks, count);
            return;
        }
    }
    printf("Task not found.\n");
}

void delete_by_serial_no(struct Task tasks[], int *count, int serial_no) {
    int i;
    for (i = 0; i < *count; i++) {
        if (tasks[i].serial_no == serial_no) {
            for (int j = i; j < *count - 1; j++) {
                tasks[j] = tasks[j + 1];
                tasks[j].serial_no--;
            }
            (*count)--;
            printf("Task deleted successfully.\n");
            save_tasks(tasks, *count);
            return;
        }
    }
    printf("Task not found.\n");
}

void delete_by_date(struct Task tasks[], int *count, int year, int month, int day) {
    int deleted = 0;
    for (int i = 0; i < *count; i++) {
        if (tasks[i].year == year && tasks[i].month == month && tasks[i].day == day) {
            for (int j = i; j < *count - 1; j++) {
                tasks[j] = tasks[j + 1];
                tasks[j].serial_no--;
            }
            (*count)--;
            deleted = 1;
            i--;
        }
    }
    if (deleted) {
        printf("Task(s) deleted successfully.\n");
        save_tasks(tasks, *count);
    } else {
        printf("No tasks found for the given date.\n");
    }
}

void print_tasks_by_date(struct Task tasks[], int count) {
    int year, month, day;
    printf("\tEnter date (YYYY MM DD): ");
    scanf("%d %d %d", &year, &month, &day);
    printf("\tTasks on %d-%02d-%02d:\n", year, month, day);
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (tasks[i].year == year && tasks[i].month == month && tasks[i].day == day) {
            print_task(tasks[i]);
            found = 1;
        }
    }
    if (!found) {
        printf("\tNo tasks found for the given date.\n");
    }
}

void print_pending_tasks(struct Task tasks[], int count) {
    printf("\tPending Tasks:\n");
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (!tasks[i].completed) {
            print_task(tasks[i]);
            found = 1;
        }
    }
    if (!found) {
        printf("No pending tasks.\n");
    }
}

int main() {
          system("color 6F");

    struct Task tasks[MAX_TASKS];
    int count = 0;
    count = load_tasks(tasks);

    int choice;
    int serial_no;
    int year, month, day;

    while (1) {
printf("\t_\n");
printf("\t^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");

printf("\t              HERE'S YOUR TO DO LIST                     \n");
printf("\t_\n");
printf("\t^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");

        printf("\tClick 1 To Create Task\n");
        printf("\tClick 2 To Mark Task as Completed\n");
        printf("\tClick 3 To Delete Task by Serial No\n");
        printf("\tClick 4 To Delete Task by Date\n");
        printf("\tClick 5 To Print Tasks by Date\n");
        printf("\tClick 6 To View Pending Tasks\n");
        printf("\tClick 7 To Exit\n");
        printf("\tEnter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            create_task(tasks, &count);
        } else if (choice == 2) {
            printf("\tEnter serial no of the task to mark as completed: ");
            scanf("%d", &serial_no);
            mark_completed(tasks, count, serial_no);
        } else if (choice == 3) {
            printf("\tEnter serial no of the task to delete: ");
            scanf("%d", &serial_no);
            delete_by_serial_no(tasks, &count, serial_no);
        } else if (choice == 4) {
            printf("\tEnter date to delete tasks (YYYY MM DD): ");
            scanf("%d %d %d", &year, &month, &day);
            delete_by_date(tasks, &count, year, month, day);
        } else if (choice == 5) {
            print_tasks_by_date(tasks, count);
        } else if (choice == 6) {
            print_pending_tasks(tasks, count);
        } else if (choice == 7) {
            printf("\tExiting...\n");
            return 0;
        } else {
            printf("\tInvalid choice. Please try again.\n");
        }
    }

    return 0;
}
