#pragma once

#include <string>
#include <vector>

class SaveSettings {
public:
  SaveSettings(const std::string& filename = "highscores.dat");
  ~SaveSettings();

  bool SaveHighScore(int score);
  
  std::vector<int> GetTopScores(int count = 5);
  
  bool ResetHighScores();

private:
  std::string m_filename;

  std::vector<int> LoadScoresFromFile();
  
  bool SaveScoresToFile(const std::vector<int>& scores);
};