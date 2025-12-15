# TP-programmation-syst-me-HANS_LAHJOUJI
**Auteurs :** HANS & LAHJOUJI 

## Explication du code

### Question 1 : Le message d'accueil
Nous affichons un message de bienvenue (avec `write` car `printf` est interdit) et le prompt initial.

```c
const char *welcome =
    "Bienvenue dans le Shell ENSEA.\n"
    "Pour quitter, tapez 'exit'.\n";

write(STDOUT_FILENO, welcome, strlen(welcome));

if (!has_status) {
    strcpy(prompt, "enseash % ");
}
write(STDOUT_FILENO, prompt, strlen(prompt));
```

### Question 2 : Exécution de commandes simples
Le shell lit la commande, crée un processus fils pour l'exécuter et attend sa fin.

```c
ssize_t n = read(STDIN_FILENO, buffer, BUF_SIZE - 1);

pid_t pid = fork();

if (pid == 0) {
    char *argv[MAX_ARGS];
    tokenize(buffer, argv, MAX_ARGS);
    if (argv[0] == NULL) _exit(0);
    execvp(argv[0], argv);
    _exit(127);
}

else if (pid > 0) {
    int status;
    if (waitpid(pid, &status, 0) > 0) {
    }
}
```

### Question 3 : Gestion de la sortie
Fonctionnement : Pour quitter le shell proprement, nous gérons deux cas :

- Ctrl+D : L'appel read renvoie 0, ce qui signifie "Fin de Fichier" (EOF).

- Commande "exit" : Nous comparons la chaîne saisie avec le mot "exit".

```c
if (n == 0) {
    write(STDOUT_FILENO, bye, strlen(bye));
    break;
}

if (strcmp(buffer, "exit") == 0) {
    write(STDOUT_FILENO, bye, strlen(bye));
    break;
}
```

### Question 4 : Affichage du code de retour
On modifie le prompt pour indiquer si la commande précédente a réussi (`exit`) ou a été interrompue par un signal (`sign`).

```c
if (WIFEXITED(status)) {
    last_exit = WEXITSTATUS(status);
} else if (WIFSIGNALED(status)) {
    last_sign = WTERMSIG(status);
}

if (last_sign != 0) {
    snprintf(prompt, sizeof(prompt),
             "enseash [sign:%d|%ldms] %% ",
             last_sign, last_time_ms);
}
else {
    snprintf(prompt, sizeof(prompt),
             "enseash [exit:%d|%ldms] %% ",
             last_exit, last_time_ms);
}
```

### Question 5 : Mesure du temps d'exécution
Fonctionnement : Nous utilisons l'horloge monotone `CLOCK_MONOTONIC` qui est insensible aux changements d'heure système. Nous relevons le temps avant le fork et après le waitpid. La différence est convertie en millisecondes pour l'affichage.

```c
struct timespec start, end;
clock_gettime(CLOCK_MONOTONIC, &start);

clock_gettime(CLOCK_MONOTONIC, &end);
long sec  = end.tv_sec  - start.tv_sec;
long nsec = end.tv_nsec - start.tv_nsec;
last_time_ms = sec * 1000 + nsec / 1000000;
```

### Question 6 : Arguments (Commandes complexes)
Fonctionnement : L'appel système execvp attend un tableau de chaînes de caractères (ex: `{"ls", "-l", NULL}`), et non une simple phrase. Nous utilisons notre fonction `tokenize` pour découper le buffer d'entrée aux espaces et remplir ce tableau `argv`.

```c
char *argv[MAX_ARGS];

tokenize(buffer, argv, MAX_ARGS);
if (argv[0] == NULL) _exit(0);
execvp(argv[0], argv);
```
