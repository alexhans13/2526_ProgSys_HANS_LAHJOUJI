//c'est pour tester l'affichage du code du signal

int main() {
    int *p = 0;
    *p = 42;   // provoque un SEGFAULT, on peut pas stocker à l'adresse 0 
    return 0;
}
