#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <getopt.h>

#include "msched-version.h"
#include "maui_utils.h"


typedef struct _showq_info {
    char  *pName;
    char  *username;
    int   idle_flag;
    int   active_flag;
    int   block_flag;
} showq_info_t;

// local convenience functions

static void free_all(showq_info_t *, client_info_t *);
static int process_args(int, char **, showq_info_t *, client_info_t *);
static void print_usage();

int main (int argc, char **argv)
{
  showq_info_t showq_info;
  client_info_t client_info;

  memset(&showq_info, 0, sizeof(showq_info));
  memset(&client_info, 0, sizeof(client_info));

  /* process all the options and arguments */
  if (process_args(argc, argv, &showq_info, &client_info)) {
          
          /* if username has been set, then only print that user's jobs*/
    if (showq_info.username != NULL && strlen(showq_info.username) != 0){
      printf("only printing %s's jobs\n", showq_info.username);
    }

    if (showq_info.block_flag) {
      puts("printing information about all jobs in blocked state");
    }

    if (showq_info.idle_flag) {
      puts("printing information about all jobs in idle state");
    }

    if (showq_info.active_flag) {
      puts("printing information about all jobs in active state");
    }

    /* if no flag has been set, print jobs in all states */
    if (showq_info.active_flag + showq_info.idle_flag + showq_info.block_flag < 1) {
      puts( "printing information about all jobs in active, idle and blocked states");
    }
  }

  free_all(&showq_info,&client_info);

  return 0;
}

/*
 converts a string to integer
 returns -1 if there exists non-digit
*/
int str_to_integer(const char *str)
{
  int n;

  if (str == NULL){
    puts("Error: input is null\n");
    return -1;
  }

  n = strlen(str);
  if (n == 0){
    puts("Error: input is empty\n");
    return -1;
  }

  for (int i = 0; i < n; i++) {
    if (isdigit(str[i])) continue;
    puts("Error: found non-digit in input\n");
    return -1;
  }

  return atoi(str);
}


/*
 processes all the arguments
 returns 1 if the option requires more action to be done
 returns 0 if no more action needs to be done
*/
int process_args(int argc, char **argv,
                 showq_info_t *showq_info,
                 client_info_t *client_info)
{
    int c;
    while (1) {
        struct option options[] = {
            {"help",        no_argument,       0, 'h'},
            {"version",     no_argument,       0, 'V'},
            {"block",       no_argument,       0, 'b'},
            {"idle",        no_argument,       0, 'i'},
            {"partition",   required_argument, 0, 'p'},
            {"running",     no_argument,       0, 'r'},
            {"username",    required_argument, 0, 'u'},
            {"configfile",  required_argument, 0, 'C'},
            {"loglevel",    required_argument, 0, 'D'},
            {"logfacility", required_argument, 0, 'F'},
            {"host",        required_argument, 0, 'H'},
            {"keyfile",     required_argument, 0, 'k'},
            {"port",        required_argument, 0, 'P'},
            {0, 0, 0, 0}
        };

        int option_index = 0;

        c = getopt_long (argc, argv, "hVbip:ru:C:D:F:H:k:P:",
                         options, &option_index);

        /* Detect the end of the options. */
        if (c == -1)
            break;

        switch (c)
        {

          case 'h':
              print_usage();

              exit(1);
              break;

          case 'V':
              printf("Maui version %s\n", MSCHED_VERSION);

              exit(1);
              break;

          case 'b':
              puts ("Show blocked queue: block_flag sets to 1\n");
              showq_info->block_flag = 1;
              break;

          case 'i':
              puts ("Show idle queue: idle_flag sets to 1\n");
              showq_info->idle_flag = 1;
              break;

          case 'p':
              printf ("set partition to %s\n", optarg);
              showq_info->pName = string_dup(optarg);
              break;

          case 'r':
              puts ("Show running queue: active_flag sets to 1\n");
              showq_info->active_flag = 1;
              break;

          case 'u':
              printf ("set username to %s\n", optarg);
              showq_info->username = string_dup(optarg);
              break;

          case 'C':
              printf ("set configfile to %s\n", optarg);
              client_info->configfile = string_dup(optarg);

              exit(1);
              break;

          case 'D':
              client_info->loglevel = string2int(optarg);

              if (client_info->loglevel != INVALID_STRING)
                  printf ("set loglevel to %s\n", optarg);

              exit(1);
              break;

          case 'F':
              printf ("set logfacility to %s\n", optarg);
              client_info->logfacility = string_dup(optarg);
              break;

          case 'H':
              printf ("set host to %s\n", optarg);
              client_info->host = string_dup(optarg);

              exit(1);
              break;

          case 'k':
              printf ("set keyfile to %s\n", optarg);
              client_info->keyfile = string_dup(optarg);

              exit(1);
              break;

          case 'P':
              client_info->port = string2int(optarg);
              if (client_info->port != INVALID_STRING)
                  printf ("set port to %s\n", optarg);

              exit(1);
              break;

          case '?':
              /* getopt_long already printed an error message. */
              puts ("Try 'showq --help' for more information.\n");
              return 0;

          default:
              abort ();
        }
    }

    return 1;
}

/* frees memory*/
void free_all(showq_info_t *showq_info, client_info_t *client_info){
    free(showq_info->username);
    free(showq_info->pName);
    free(client_info->configfile);
    free(client_info->host);
    free(client_info->keyfile);
    free(client_info->logfacility);
}

void print_usage()
{
    puts ("\nUsage: showq [FLAGS]\n\n"
          "Display information about all jobs in active, idle and blocked states.\n"
          "\n"
          "  -h, --help                     display this help\n"
          "  -V, --version                  display client version, serverhost, serverport, home\n"
          "                                   directory, build location, build host and build date\n"
          "\n"
          "  -b, --blocked                  display blocked jobs only\n"
          "  -i, --idle                     display idle jobs only\n"
          "  -r, --running                  display active jobs only\n"
          "  -p, --partition=PARTITIONID    display jobs assigned to the specified partition\n"
          "  -u, --username=USERNAMEID      display information about jobs for the specified user\n"
          "\n"
          "  -C, --configfile=FILENAME      set configfile\n"
          "  -D, --loglevel=LOGLEVEL        set loglevel\n"
          "  -F, --logfacility=LOGFACILITY  set logfacility\n"
          "  -H, --host=SERVERHOSTNAME      set serverhost\n"
          "  -k, --keyfile=FILENAME         set server keyfile\n"
          "  -P, --port=SERVERPORT          set serverport\n");
}