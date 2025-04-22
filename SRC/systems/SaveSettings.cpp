#include <systems/SaveSettings.h>
#include <iostream>
#include <fstream>
#include <algorithm>

SaveSettings::SaveSettings(const std::string& filename) 
  : m_filename(filename) {
}

SaveSettings::~SaveSettings() {
}

bool SaveSettings::SaveHighScore(int score) {
  std::vector<int> scores = LoadScoresFromFile();
  
  scores.push_back(score);
  
  std::sort(scores.begin(), scores.end(), std::greater<int>());

  return SaveScoresToFile(scores);
}

std::vector<int> SaveSettings::GetTopScores(int count) {
  std::vector<int> scores = LoadScoresFromFile();
  
  std::sort(scores.begin(), scores.end(), std::greater<int>());
  
  if (scores.size() > static_cast<size_t>(count)) {
    scores.resize(count);
  }
  
  return scores;
}

bool SaveSettings::ResetHighScores() {
  std::vector<int> emptyScores;
  return SaveScoresToFile(emptyScores);
}

std::vector<int> SaveSettings::LoadScoresFromFile() {
  std::vector<int> scores;
  std::ifstream file(m_filename, std::ios::binary);
  
  if (!file.is_open()) {
    std::cout << "Could not open scores file for reading: " << m_filename << std::endl;
    return scores; 
  }
  
  int score;
  while (file.read(reinterpret_cast<char*>(&score), sizeof(int))) {
    scores.push_back(score);
  }
    
  file.close();
  return scores;
}

bool SaveSettings::SaveScoresToFile(const std::vector<int>& scores) {
  std::ofstream file(m_filename, std::ios::binary | std::ios::trunc);
  
  if (!file.is_open()) {
    std::cout << "Could not open scores file for writing: " << m_filename << std::endl;
    return false;
  }
  
  for (int score : scores) {
    file.write(reinterpret_cast<const char*>(&score), sizeof(int));
  }
  
  file.close();
  return true;
}