#define _XOPEN_SOURCE
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

struct user {
    char username[50];
    char password[256];
    char firstname[50];
    char lastname[50];
    int admin;
};

char* cs521Hash(const char* password) {
    return crypt(password, "00");
}

struct user* createUsers(int* count) {
    int lines=0;
    FILE* fp; 
    fp=fopen("credential_file.dms", "r");
    if (fp == NULL) {
        printf("Error in opening file.\n");
        return NULL;
    }
    char ch;
    while ( ( ch = getc(fp) ) != EOF) {
            if(ch=='\n'){
            lines++;
          }
    }
    struct user* users=(struct user*)malloc(lines*sizeof(struct user));
    *(count)+=lines;
    return users;
    fclose(fp);
}

void populateUsers(void* users) {
  FILE* fp;
  fp=fopen("credential_file.dms", "r");
  if (fp == NULL) {
    printf("Error in opening file.\n");
    return;
  }
  char line[sizeof(struct user)];
  int index=0;
  while(fgets(line, sizeof(line), fp)){
    struct user* u=users+index;
    char delim [2] = "\t";
    char* firstname=strtok(line, delim);
    char* lastname=strtok(NULL, delim);
    char* username = strtok(NULL, delim);
    char* password=strtok(NULL, delim);
    char* admin=strtok(NULL, delim);
    int nAdmin=*admin-'0';

    strcpy(u->firstname, firstname);
    strcpy(u->lastname,lastname);
    strcpy(u->username, username);
    strcpy(u->password, password);  
    u->admin=nAdmin;

    // printf("%s\n", u->firstname); 
    // printf("%s\n", lastname);
    // printf("%s\n", username);
    // printf("%s\n", password);
    // printf("%d\n", u->admin);
    index++;
  }
  fclose(fp);
}

int checkAdminPassword(char* password, struct user* users, int count) {
    for (int i = 0; i < count; ++i) {
        if (strcmp((users + i)->username, "admin") == 0) {
            if (strcmp("s#1Pa5",password)==0) {
                return 1;
            }
            else {
                return 0;
            }
        }
    }
    return 0;
}

struct user* addUser(struct user* users, int* count, char* username, char* password, char* firstname, char* lastname, int admin) {
    *count=*count+1;
    users=realloc(users, *(count)*sizeof(struct user));
    struct user* newUser=users+*count-1;
    strcpy(newUser->username, username);
    strcpy(newUser->password, cs521Hash(password));
    strcpy(newUser->firstname, firstname);
    strcpy(newUser->lastname, lastname);
    newUser->admin=admin;
    return users;
}

void saveUsers(struct user* users, int count) {
  FILE* fp; //declaring file pointer
  //Opening the file in write mode
    fp = fopen("credential_file.dms", "w");

    if (fp == NULL) {
        printf("Error in opening file.\n");
        return;
    }
  int i;
  for(i=0;i<count;i++){
    char* firstname=(users+i)->firstname;
    char* lastname=(users+i)->lastname;
    char* username=(users+i)->username;
    char* password=(users+i)->password;
    char admin='0'+(users+i)->admin;
    fputs(firstname, fp);
    fputc('\t', fp);
    fputs(lastname, fp);
    fputc('\t', fp);
    fputs(username, fp);
    fputc('\t', fp);
    fputs(password, fp);
    fputc('\t', fp);
    fputc(admin, fp);
    fputc('\n', fp);
  }
  fclose(fp);
}

int main(void) {
    int user_count = 0;
    struct user* users = createUsers(&user_count);
    if (users == NULL) {
        return EXIT_FAILURE;
    }
    populateUsers(users);
    printf("Enter admin password to add new users:\n");
    char entered_admin_password[50];
    scanf(" %s", entered_admin_password);
    if (checkAdminPassword(entered_admin_password, users, user_count)) {
        struct user new_user;
        printf("Enter username:");
        scanf(" %s", new_user.username);
        printf("Enter first name:");
        scanf(" %s", new_user.firstname);
        printf("Enter last name:");
        scanf(" %s", new_user.lastname);
        printf("Enter password:");
        scanf(" %s", new_user.password);
        printf("Enter admin level:");
        scanf(" %d", &(new_user.admin));
        users = addUser(users, &user_count, new_user.username, new_user.password, new_user.firstname, new_user.lastname, new_user.admin);
        if (users == NULL) {
            return EXIT_FAILURE;
        }
    }
    saveUsers(users, user_count);
    free(users);
    return EXIT_SUCCESS;
    // char entered_admin_password[50];
    // scanf(" %s", entered_admin_password);
    // printf("%s",cs521Hash(entered_admin_password));
}
