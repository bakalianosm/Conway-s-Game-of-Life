#include <stdio.h>
#include "ADTSet.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "life.h"


//αυτή η συνάρτηση,είναι η πρώτη compare  που έφτιαξα , και την αφηνω εδώ.
// int compare(Pointer a, Pointer b){ //εδώ έχω φτιάξει μία συνάρτηση που συγκρίνει τα κύτταρα.
//     if((((LifeCell*)a)->x == ((LifeCell*)b)->x) && (((LifeCell*)a)->y == ((LifeCell*)b)->y))
//         return 0;                           //αν είναι ίσα επέστρεψε 0
//     else if(((LifeCell*)a)->y < ((LifeCell*)b)->y || //διαφορετικά,έχω κάνει μια σύμβαση για την σύγκριση των κελιών.
//        (((LifeCell*)a)->y > ((LifeCell*)b)->y && ((LifeCell*)a)->x < ((LifeCell*)b)->x)) return 1;
//     else return -1;
// }

int compare(Pointer a, Pointer b){ //εδώ έχω φτιάξει μία συνάρτηση που συγκρίνει τα κύτταρα.
    if((((LifeCell*)a)->x == ((LifeCell*)b)->x) && (((LifeCell*)a)->y == ((LifeCell*)b)->y))
        return 0;                           //αν είναι ίσα επέστρεψε 0
   else if( (((LifeCell*)a)->x > ((LifeCell*)b)->x) && (((LifeCell*)a)->y == ((LifeCell*)b)->y)  ) return 2;
   else if(  (((LifeCell*)a)->x == ((LifeCell*)b)->x)  && (((LifeCell*)a)->y > ((LifeCell*)b)->y)  ) return 3;
   else if(  (((LifeCell*)a)->x < ((LifeCell*)b)->x)  && (((LifeCell*)a)->y > ((LifeCell*)b)->y)  ) return 3;
   
    else return -1;
}
void iterate_and_print(LifeState temp){
  for(SetNode node = set_first(temp);
        node != SET_EOF;
        node = set_next(temp, node)){
            printf("Set contains (%d,%d) \n" , ((LifeCell*)set_node_value(temp, node))->x , ((LifeCell*)set_node_value(temp, node))->y) ;
    } 
    
}

int find_max_x(LifeState temp){
  int max=0;
  for(SetNode node = set_first(temp);
        node != SET_EOF;
        node = set_next(temp, node)){
            if(((LifeCell*)set_node_value(temp, node))->x > max) max=((LifeCell*)set_node_value(temp, node))->x;
    }
    
    return max;
}

int find_max_y(LifeState temp){
  int max=0;
  for(SetNode node = set_first(temp);
        node != SET_EOF;
        node = set_next(temp, node)){
            if(((LifeCell*)set_node_value(temp, node))->y > max) max=((LifeCell*)set_node_value(temp, node))->y;
        }
        
    return max;
}

int num_of_digits(int num){
	int digits=1;
	while(num/10!=0){
        digits++;
        num=num/10;
    }
    //printf("%d",digits);
	return digits; 
}

int get_neighbours_num(LifeState state , LifeCell* cell){  
  int i=(cell->x); 
  int j=(cell->y);
  int counter=0; //μετρητής,μετράει πόσους ζωντανούς γείτονες έχει το κύτταρο cell
  LifeCell* tempCell=malloc(sizeof(LifeCell));
  for( int y=j-1 ; y<= j+1 ; y++){ // για κάθε τιμή του x
    for (int x=i-1 ; x<=i+1 ; x++){ //για κάθε τιμή του y
      // printf("mesa stin alive neighbours : X is %d\t Y is %d\t\n",x,y);
      if(x==cell->x &&y==cell->y)  continue; //συνέχισε με το επόμενο,αν το κύτταρο είναι
                                  // το κύτταρο του οποίου θελουμε να βρούμε τους γείτονες
      else{
        tempCell->x=x;
        tempCell->y=y;
        //printf("tempcell x is %d tempcell y is %d\n",tempCell->x,tempCell->y);
        if(set_find(state,(LifeCell*)tempCell)!=NULL){
           //printf("\t\t\tkai einai zontanos \n");
           counter++; //αν είναι ζωντανό , αύξησε τον μετρητή
        }
      }
    }
  }
  free(tempCell);
  return counter ; // επέστρεψε τον μετρητή
}

char* load_file(char const* path)
{
    char* buffer = 0;
    long length;
    FILE * f = fopen (path, "rb"); //was "rb"

    if (f)
    {
      fseek (f, 0, SEEK_END);
      length = ftell (f);
      fseek (f, 0, SEEK_SET);
      buffer = (char*)malloc ((length+1)*sizeof(char));
      if (buffer)
      {
        fread (buffer, sizeof(char), length, f);
      }
      fclose (f);
    }
    buffer[length] = '\0';
    return buffer;
}

LifeState life_create(){ //αρχικοποιώ ,  δημιουργώ τον τύπο ΑΔΤ που επέλεξα να αναπαραστήσει την LifeState
	LifeState alive_cells_set = set_create(compare,free); //απλά δημιουργώ ένα κενό Set.
	// printf("Lifestate Created \n");
    printf("\033[1;32mLifeState Created.\n\033[0m"); //εκτυπώνω μήνυμα .
	return alive_cells_set ;
}

LifeState life_create_from_rle(char* file){

  int i=0;
  int multiplier=0; //  ο "πολλαπλασιαστής" , ουσιαστικά μετράει πόσους ίδιους χακτήρες απο (o,b) έχουμε και το προσθέτει πρίν από αυτούς.
  int x=1; 
  int y=1;
  int alive_cell_counter=0; //μετρητής,χρησιμοποιείται για να μετρήσω πόσα ζωντανά κελιά έχω(είναι για δική μου ευκολία σε debugging)
  int max_x=0;
  LifeState alive_cells=set_create(compare,free);



  while(file[i]!='!'){
     while(file[i]=='\n') { //επειδή μπορεί το πρόγραμμα να διαβάσει μεγάλα αρχεία τα οποία
        i++;               // ενδέχεται να περιέχουν πολλές αλλαγές γραμμής,τα αγνοεί.
      continue;            // και συνεχίζει με τον επόμενο χαρακτήρα.
     }
    if(isdigit(file[i])){ //χρησιμοποιώ την συνάρτηση αυτή για να ελέγξω αν ο χακατήρας που διαβάστηκε είναι ψηφίο.
      //digit
      multiplier=multiplier*10 + (file[i]-'0'); //αν είναι, πολλαπλασίασε το με 10 και φτιάξε τον αντίστοιχο ακέραιο 
      i++;  //συνέχισε με τον επόμενο χαρακτήρα.
    }

    else if(file[i]=='o'){ //αν ο χαρακτήρας είναι το "o" αναφερόμαστε σε ζωντανό κύτταρο 
      // o
      if(multiplier>0){ //ελέγχουμε αν ο "πολλαπλασιαστής" είναι >0 , δηλαδή αν υπάρχει αριθμός πριν
        
        for (int j=0 ; j <multiplier ; j++){
          LifeCell* toInsert=malloc(sizeof(LifeCell)); //αν υπάρχει,φτιάξε τόσα κύτταρα με τα αντίστοιχα x,y 
          toInsert->x=x;
          toInsert->y=y;
          set_insert(alive_cells,(LifeCell*)toInsert); //προσέθεσε τα στο Set , στο LifeState
          //iterate_and_print(alive_cells);
          alive_cell_counter++; //αύξησε τα πλήθος των ζωντανών κατά 1.
          x++; //αύξησε το x 
        }
        multiplier=0; //περάσαμε multiplier ζωντανά κύτταρα στη κατάσταση, πρέπει να επαναφέρουμε στο 0
                     //τον πολλαπλασιαστή,ώστε να μην έχει κρατήσει τιμή όταν ξανά υπάρχει ψηφίο.
      }
      else{ //αλλιώς άν ο πολλαπλασιαστής είναι 0,σημαίνει ότι έχουμε μόνο ΕΝΑ ζωντανό κελί 
        alive_cell_counter++; //άς αυξησουμε τον μετρητή των ζωντανών κελιών
        LifeCell* toInsert=malloc(sizeof(LifeCell)); //και στη συνέχεια να το προσθέσουμε στη κατάσταση
        toInsert->x=x;
        toInsert->y=y;
        //printf("Inserting (%d,%d)\t...",toInsert->x,toInsert->y);
        set_insert(alive_cells,(LifeCell*)toInsert);
        // iterate_and_print(alive_cells);
        x++; //αύξησε το x.
      }
      i++; //συνέχισε με τον επόμενο χαρακτήρα.
    }

    else if(file[i]=='b'){ ///αν ο χαρακτήρας είναι το "b" αναφερόμαστε σε νεκρό κύτταρο 
      // b
      if (multiplier > 0){//ελέγχουμε αν ο "πολλαπλασιαστής" είναι >0 , δηλαδή αν υπάρχει αριθμός πριν .
        for (int j=0 ; j <multiplier ; j++){ //αυτή η επανάληψη είναι αυστηρά για να εκτυπώνει τόσες φορές 
                                            //όσες ο πολλαπλασιαστής,πόσα είναι τα νεκρά κύττρα
                                            //καθώς και τις συντεταγμένες τους.Δεν χρησιμεύει στην υλοποίση.
          x++;
        }
        multiplier=0; //τελειώσαμε,δεν χρειάζεται η υπάρχουσα τιμή και το επαναφέρουμε στο 0.
      }
      else{ // //αλλιώς άν ο πολλαπλασιαστής είναι 0,σημαίνει ότι έχουμε μόνο ΕΝΑ νεκρό κελί 
          x++; //αύξησε το x.
      }
      i++; //συνέχισε με τον επόμενο χαρακτήρα.
    }
    else if(file[i]=='$'){ //αν ο χαρακτήρας είναι το "$" αναφερόμαστε σε αλλαγή γραμής 
      //$
      if(x>max_x) max_x=x;
      //printf("\t**NEWLINE**\n");
      y++; //αυξανεται το y εφόσον άλλαξε η γραμμή.
      x=1; //το x παίρνει την αρχική τιμη, το αριθμό 1 (όπως έχει γίνει η σύμβαση , βλ. README).
      i++; //συνέχισε με τον επόμενο χαρακτήρα.
    }
    else {
      //αν ο ζαρακτήρας που διαβάστηκε δεν ανήκε στο σύνολο { [0,..,9] , o , b , $ }
      printf("\033[1;31mSORRY , INVALID INPUT \n\033[0m");
      return 0; //επέστρεψε.Λάθος είσοδος.
    }
  
  }
  max_x--;
  printf("\033[1;32mLifeState Created from RLE file.\n\033[0m");
  return alive_cells;
}

void life_save_to_rle(LifeState state, char* file){

  int x_bound , y_bound , str_counter=0 ; // αρχικοποίηση κάποιων μεταβλητών που θα μας χρειαστούν στην υλοποίηση
                                            // της συνάρτησης αυτής
  // char *file;

  //βρίσκω τα "όρια"  των x,y  
  x_bound=find_max_x(state); 
  y_bound=find_max_y(state);

  //φτιάχνω μια μεταβλητή ακόμα, που θα αποθηκεύσει τη συμβολοσειρά, χωρίς να είναι σε μορφή RLE.
  char *str=malloc(((x_bound)*(y_bound))*sizeof(char));
  
  //δεσμεύω χώρο,στην μεταβλητή που έρχεται σαν όρισμα , τόσο όσο οι "διαστάσεις" του συστήματος
  //συντεταγμένων που περιλαμβάνει την κατάσταση.
  //πχ μια κατάσταση νxν αποτελείται το πολύ απο ν^2 χαρακτήρες.(πχ αν έιναι όλα ζωντανά)
  //στην υλοποίηση μου, αποτελείται ΑΚΡΙΒΩΣ από ν^2 χαρακτήρες    
  file=malloc(((x_bound)*(y_bound))*sizeof(char));
  
  LifeCell tempCell;
    
  for(int y=1; y<=y_bound ; y++){ //αρχίζω να τσεκάρω κάθε σημείο το οποίο υπάρχει στην κατάσταση
    for (int x=1 ; x<=x_bound ; x++){
        tempCell.x=x;
        tempCell.y=y;
      //if(isAlive(state,x,y)) {  //αν είναι ζωντανό 
      if(life_get_cell(state,tempCell)){ //αν είναι ζωντανό 
        //printf("o") ;           
        str[str_counter]='o' ;//'o'; //στην συμβολοσειρά γίνεται append το "o"
      }
      else{ //αλλιώς άν είναι νεκρό
       // printf("b") ; 
        str[str_counter]='b' ; //στην συμβολοσειρά γίνεται append το "b"
      }
      str_counter++; //αυξάνεται ο μετρητής της συμβολοσειράς,ώστε να γράψουμε στο επόμενο στοιχειο.
    }
    
    //βγήκε από την εμφωλευμένη for 
    if(y<y_bound) {//αν το y είναι μικρότερο από το μέγιστο 
      //printf("$")  ;
      str[str_counter]='$' ; //στην συμβολοσειρά γίνεται append το "$"
    }
    else{ //αν το y είναι ίσο με το μέγιστο y 
      //printf("!") ; str[str_counter]='!' ; ////στην συμβολοσειρά γίνεται append το "!"
    }
    str_counter++; //αυξάνεται ο μετρητής της συμβολοσειράς,ώστε να γράψουμε στο επόμενο στοιχειο.
  }

    //τώρα,έχουμε φτιάξει την συμβολοσειρά , η οποία είναι της μορφής πχ. "obo$oobboo$oobo!"
    //σε αυτο το κομμάτι , βρίσκω τις επαναλήψεις των ομοίων στοιχείων και τις αντικαθιστώ με έναν αριθμό .
  int n = str_counter;
  // char ptr[x_bound*y_bound] ;

  char ptr[100000]; // ένας πίνακας ο οποίος μας κατάει την συμβολοσειρά rle που θα φτιαχτεί.
  for (int i = 0; i < n; i++) { 
      int count = 1; //ο μετρητής αυτός μετράει πόσες φορές έχει διαβάσει τον ίδιο χαρακτήρα σε σειρά
      while (i < n - 1 && str[i] == str[i + 1]) { //όσο το επόμενο στοιχείο απ το τρέχον είναι ο ίδιος χαρακτήρας
        count++;  //αύξησε τον μετρητή
        i++; 
      } 

      if (count>1){//αν υπάρχουν πάνω από 1 ίδιοι χαρακτήρες
          int num=num_of_digits(count); //βρίσκει πόσα ψηφία έχει ο αριθμός (πχ ο αριθμός 230 έχει 3 ψηφια)
          char buf[num]; //φτιαχνω έναν πίνκακα από χαρακτήρες,τόσους όσα τα ψηφία του count
          sprintf(buf,"%d",count); //με τη χρήση αυτής της συνάρτησης μετατρέπω τον αριθμό count σε συμβολοσειρά 
          strcat(ptr,buf); //προσθέτω τη συμβολοσειρα αυτή,δηλαδή τον αριθμό count ,τη συνενώνω με την αρχικη-υπόλοιπη συμβολοσειρά
          strncat(ptr,&str[i],1); //προσθέτω και τον χαρακτήρα.
      }
      else {
          strncat(ptr,&str[i],1); //έχουμε μόνο ένα χαρακτήρα στην σειρά,προσέτουμε μόνο τον χαρακτηρα στο τέλος της συμβολοσειράς.
      }
    
  } 
   printf("Just producted the string :\t%s\n",ptr);
  free(str); //απελευθερώνω και τη μνήμη αυτής της μεταβλητής
  //τέλος έχει δημιουργήθεί αρχέιο της μορφής, πχ "obo$2o2b2o$2obo!"
  //και το επιστρέφει.
  
}


bool life_get_cell(LifeState state, LifeCell cell){
  if(set_find(state,&cell)!=NULL) return true; //αν υπάρχει , είναι ζωντανό
  return false; //δεν βρεθηκε , είναι νεκρό
}

void life_set_cell(LifeState state, LifeCell cell, bool value){
  if(value==true){ // αν θέλει να ζωντανέψω ένα κελί
    set_insert(state,&cell);  //το προσθέτω και δεν χρειάζεται να ελέγξω αν υπάρχει γιατι το
                              //κάνει κατά κάποιο τρόπο η set_insert .
  }
  else{ //αν θέλει να νεκρώσω ένα κελί
    if(set_find(state,&cell)!=NULL) set_remove(state,&cell); //τότε άν υπάρχει, το αφαιρώ                                
                                                            //αλλιώς αν δεν υπάρχει,δε μας ενδιαφέρει.
  }
}

LifeState life_evolve(LifeState state){
  printf("Evolving...\n");
  LifeState new_state=life_create(); // δημιουργώ την κατάσταση που θα επιστραφεί
  int x_bound=find_max_x(state); //το "όριο" του x
  int y_bound=find_max_y(state); //το "όριο" του y
  int neighbours=0;
  LifeCell* tempCell=malloc(sizeof(LifeCell));
  for(int y=1 ; y<=y_bound ; y++){  //για κάθε τιμή του x
    for(int x=1 ; x<=x_bound ; x++){ //για κάθε τιμή του υ
      tempCell->x=x; //φτιάχνω το προσωρινό κελί
      tempCell->y=y;
      neighbours=get_neighbours_num(state,tempCell); //βρίσκω πόσους ζωντανούς γείτονες έχει
      if((neighbours <2 || neighbours >3 )){ //αν έχει λίγοτερους από 2 ή περισσότερους από 3 ζωντανούς
        life_set_cell(new_state,*tempCell,false); //το σκοτώνω
      }
      else if(neighbours==2){
         LifeCell* toInsert=malloc(sizeof(LifeCell)); //το φτιάχνω
        toInsert->x=x;
        toInsert->y=y;
        set_insert(new_state,toInsert); //και το βάζω
      }
      else if( (set_find(state,tempCell)!=NULL) && neighbours==3){ //αν είναι ζωντανό και έχει 3 ζωντανούς γείτονες
                                                          //γείτονες , θα το βάλει στη νέα κατάσταση
        LifeCell* toInsert=malloc(sizeof(LifeCell)); //το φτιάχνει
        toInsert->x=x;
        toInsert->y=y;
        set_insert(new_state,toInsert); //και το τοποθετέι
      }
      else if(life_get_cell(state,*tempCell)){
     // else if (isAlive(state,x,y)) { //διαφορετικά άν είναι ζωντανό 
        LifeCell* toInsert=malloc(sizeof(LifeCell)); //το φτιάχνω
        toInsert->x=x;
        toInsert->y=y;
        set_insert(new_state,toInsert); //και το βάζω
      }
    }
  }
  printf("\033[1;33mThe size of the evolved state is %d\n\033[0m",set_size(new_state));
  free(tempCell);
  return new_state; //και την επιστρέφω.
}

//ΔΕΝ ΞΕΡΩ ΚΑΤΑ ΠΟΣΟ ΧΡΕΙΑΖΕΤΑΙ ΕΔΩ.
int isAlive(LifeState ls, int x , int y){
  LifeCell* cell=malloc(sizeof(LifeCell));
  cell->x=x;
  cell->y=y;
  if(set_find(ls,cell)!=NULL) {
    free(cell);
    return 1;
  }
  else{
    free(cell);
    return 0;
  }
}

void life_destroy(LifeState temp){
  set_destroy(temp);
}
