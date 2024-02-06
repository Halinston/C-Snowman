#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <string>
#include <random>
#include <chrono>
#include <thread>

class Snowman {
private:
    int partsRemaining;

public:
    Snowman() : partsRemaining(7) {}

    void display() const {
        // ASCII art display of the snowman based on remaining parts
        std::cout << "  *  -=[ snowman ]=-  " << partsRemaining << "/7" << std::endl;
        std::cout << "       *     *" << std::endl;
        std::cout << "             ___   *" << std::endl;
        std::cout << "       *   _|___|_      *" << std::endl;
        std::cout << "  *       '=/a a\\='  *" << std::endl;
        std::cout << "      *     \\~_ /        *" << std::endl;
        std::cout << "    *  _\\__/ '-' \\__/_   *" << std::endl;
        std::cout << "        /  \\  o  /  \\" << std::endl;
        std::cout << "  *       / '---' \\   *" << std::endl;
        std::cout << "     jgs |    o    |      *" << std::endl;
        std::cout << "   ---.---\\   o   /-----.---" << std::endl;
        std::cout << "           '-----'`" << std::endl;
    }

    void melt() {
        partsRemaining--;

        // Simulate melting by displaying the updated snowman
        system("CLS");  // Clear the console for a clean display
        display();
    }

    bool isMelted() const {
        return partsRemaining <= 0;
    }
};

class Word {
private:
    std::string mysteryWord;
    std::vector<bool> guessedLetters;

public:
    Word(const std::string& word) : mysteryWord(word) {
        guessedLetters.resize(mysteryWord.length(), false);
    }

    bool guessLetter(char letter) {
        bool found = false;
        for (int i = 0; i < mysteryWord.length(); ++i) {
            if (mysteryWord[i] == letter) {
                guessedLetters[i] = true;
                found = true;
            }
        }
        return found;
    }

    bool isWordGuessed() const {
        return std::all_of(guessedLetters.begin(), guessedLetters.end(), [](bool val) { return val; });
    }

    void displayProgress() const {
        for (int i = 0; i < mysteryWord.length(); ++i) {
            if (guessedLetters[i]) {
                std::cout << mysteryWord[i];
            }
            else {
                std::cout << "_";
            }
        }
        std::cout << std::endl;
    }

    const std::string& getMysteryWord() const {
        return mysteryWord;
    }
};

class Game {
private:
    Snowman snowman;
    Word word;

public:
    Game(const std::string& mysteryWord) : word(mysteryWord) {}

    void play() {
        while (!word.isWordGuessed() && !snowman.isMelted()) {
            displayGameStatus();
            char guess;
            std::cout << "Enter your guess: ";
            std::cin >> guess;

            if (word.guessLetter(guess)) {
                std::cout << "Correct guess!\n";
            }
            else {
                std::cout << "Incorrect guess!\n";
                snowman.melt();
            }
        }

        displayGameResult();
    }

    void displayGameStatus() const {
        system("CLS");  // Use this for Windows, you might need to adjust for other platforms
        snowman.display();
        word.displayProgress();
    }

    void displayGameResult() const {
        if (word.isWordGuessed()) {
            std::cout << "Congratulations! You guessed the word!\n";
        }
        else {
            std::cout << "Sorry, the snowman melted. You lose!\n";
            std::cout << "The mystery word was: " << word.getMysteryWord() << std::endl;
        }
    }
};

std::vector<std::string> readWordListFromFile(const std::string& filename) {
    std::vector<std::string> wordList;
    std::ifstream file(filename);

    if (file.is_open()) {
        std::string word;
        while (std::getline(file, word)) {
            wordList.push_back(word);
        }
        file.close();
    }
    else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }

    return wordList;
}

std::string getRandomWord(const std::vector<std::string>& wordList) {
    if (wordList.empty()) {
        std::cerr << "No words available." << std::endl;
        return "";  // Return an empty string if the word list is empty
    }

    // Use std::shuffle to shuffle the word list and then pick the first word
    std::vector<std::string> shuffledWords = wordList;
    std::shuffle(shuffledWords.begin(), shuffledWords.end(), std::default_random_engine(std::time(nullptr)));

    return shuffledWords[0];
}

int main() {

    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    std::vector<std::string> wordList = readWordListFromFile("wordlist.txt");

    std::string mysteryWord = getRandomWord(wordList);

    if (mysteryWord.empty()) {
        std::cerr << "No word selected. Exiting..." << std::endl;
        return 1;
    }

    Game snowmanGame(mysteryWord);
    snowmanGame.play();

    return 0;
}
