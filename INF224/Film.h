#pragma once

#include "Video.h"

class Film : public Video {

  private:
      int* chapters;
      int numberOfChapters;

  public:
      virtual ~Film() { delete[] chapters; }

      Film(): Video(), chapters(NULL), numberOfChapters(0) { }
      Film(std::string name, std::string pathname, int _length, int* _chapters,
          int _numberOfChapters): Video(name, pathname, length),
          numberOfChapters(_numberOfChapters) {

          chapters = new int[numberOfChapters];
          std::copy_n(_chapters, numberOfChapters, chapters);
      }

      const int getNumberOfChapters() { return numberOfChapters; }
      const int* getChapters() {
        // C'est assez sale ce que tu as fait. D'abord tout ce que tu crées avec new tu dois normalement le détruire avec delete, 
        // l'os le fera pour toi si tu oublies mais c'est sale. La quand t'appelles la méthode depuis l'extérieur tu dois faire 
        // const int* chapters = obj.getChapters(); puis quand t'as fini d'utiliser l'objet tu dois faire delete chapters depuis l'extérieur
        // C'est moche...
        // Donc tu peux ajouter un paramètre : getChapters(const int* chapters); puis éventuellement faire une copie
         
      }

      const void displayChapters(std::ostream& os) {

          for(int i = 0; i < numberOfChapters; i++) {

            os << "Le chapitre : " << i << " dure "
            << chapters[i] << " secondes" << std::endl;
          }
      }

};
