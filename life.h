#include <stdio.h>
#include "ADTSet.h"
#include <string.h>
#include <stdlib.h>
// #include <ctype.h>

typedef struct {
	int x, y;
} LifeCell;

typedef Set LifeState;

// Η συγκριτική συνάρτηση που χρειάζεται.
int compare(Pointer a, Pointer b);

// Διατρέχει όλοκληρο το SET και εκτυπώνει τις τιμές των κελιών που περιέχει.
void iterate_and_print(LifeState temp);

// Βρίσκει τη μεγαλύτερη τιμή που θα παίρνει το x.
int find_max_x(LifeState temp);

// Βρίσκει τη μεγαλύτερη τιμή που θα παίρνει το y.
int find_max_x(LifeState temp);

int isAlive(LifeState ls , int x , int y);

// Βρίσκει από πόσα ψηφία αποτελείται ο αριθμός num,και τα επιστρέφει.
int num_of_digits(int num);

// Παίρνει όρισμα ένα αρχείο κειμένου και επιστρέφει  το αρχείο σε μορφή συμβολοσειράς.
char* load_file(char const* path);

//παίρνει ώς όρισμα ένα κελί και μια κατάσταση και μας επιστρέφει τον αριθμό
// των ΖΩΝΤΑΝΩΝ γειτόνων του κελιού.
int get_neighbours_num(LifeState state , LifeCell* cell);

// Δημιουργεί μια κατάσταση του παιχνιδιού όπου όλα τα κελιά είναι νεκρά.
LifeState life_create();

// Δημιουργεί μία κατάσταση του παιχνιδιού με βάση τα δεδομένα του αρχείο file (RLE format)
LifeState life_create_from_rle(char* file);

// Αποθηκεύει την κατάσταση state στο αρχείο file (RLE format)
void life_save_to_rle(LifeState state, char* file);

// Επιστρέφει την τιμή του κελιού cell στην κατάσταση state (true: ζωντανό, false: νεκρό)
bool life_get_cell(LifeState state, LifeCell cell);

// Αλλάζει την τιμή του κελιού cell στην κατάσταση state
void life_set_cell(LifeState state, LifeCell cell, bool value);

// Παράγει και επιστρέφει μια νέα κατάσταση που προκύπτει από την εξέλιξη της κατάστασης state.
// Η ίδια η state δε μεταβάλλεται (ούτε καταστρέφεται).
LifeState life_evolve(LifeState state);

// Καταστρέφει την κατάσταση ελευθερώντας οποιαδήποτε μνήμη έχει δεσμευτεί
void life_destroy(LifeState state);


//ΔΕΝ ΞΕΡΩ ΚΑΤΑ ΠΟΣΟ ΧΡΕΙΑΖΕΤΑΙ ΕΔΩ.
int isAlive(LifeState ls, int x , int y);
