//
// Created by Shira on 6/25/2023.
//
#include "RecommendationSystem.h"
#include <cmath>
#include <set>
#include <algorithm>
#include <cfloat>

typedef std::pair<double, sp_movie> t_pair;

sp_movie RecommendationSystem::add_movie(const std::string& name,int year,
                                         const std::vector<double>& features)
{
  sp_movie new_sp_movie= std::make_shared<Movie>(name,year);
  main_ranking_map[new_sp_movie] = features;
  return new_sp_movie;
}

/**
 * Calculates the Euclidean norm of a given vector.
 * @param vec The input vector.
 * @return The Euclidean norm of the vector.
 */
double norm(const std::vector<double>& vec)
{
  double summing = 0;
  for (double p : vec)
  {
    summing += (p * p);
  }
  return std::sqrt(summing);
}

/**
 * Calculates the dot product of two vectors.
 * @param vec1 The first input vector.
 * @param vec2 The second input vector.
 * @return The dot product of the two vectors.
 */
double dot_product(const std::vector<double>& vec1, const
std::vector<double>& vec2)
{
  double res=0;
  for (int p=0;p<(int)vec1.size();p++)
  {
    res += (vec1[p]*vec2[p]);
  }
  return res;
}

/**
 * Calculates the cosine similarity (angle) between two vectors.
 * @param vec1 The first input vector.
 * @param vec2 The second input vector.
 * @return The cosine similarity between the two vectors.
 */
double ang_calc(const std::vector<double>& vec1, const
std::vector<double>& vec2)
{
  return (dot_product(vec1,vec2)) / (norm(vec2) * norm(vec1));
}


sp_movie RecommendationSystem::recommend_by_content(const User& user)
{
  rank_map temp_rank_map = user.get_ranks ();
  double sum = 0;
  int num_of_movies = 0;
  for (auto &it: temp_rank_map)
  {
      sum += it.second;
      num_of_movies += 1;
  }
  double avr = sum / num_of_movies;
  for (auto &it: temp_rank_map)
  {
      it.second -= avr;
  }
  unsigned long long int len_vec =
      (main_ranking_map[user.get_ranks().begin ()->first]).size ();
  std::vector<double> pref_vec(len_vec);
  for (auto &it: temp_rank_map)
  {
    for (int i = 0; i < (int)len_vec; i++)
    {
      pref_vec[i]+=((main_ranking_map[it.first])[i] * (it.second));
    }
  }
  sp_movie res_movie;
  double prob = -DBL_MAX;
  for (auto &it: main_ranking_map)
  {
    if(temp_rank_map.find(it.first) == temp_rank_map.end())
    {
      double temp_prob = ang_calc
          (pref_vec, main_ranking_map[it.first]);
      if (temp_prob > prob)
      {
        prob = temp_prob;
        res_movie = it.first;
      }
    }
  }
  return res_movie;
}

/**
 * Compares two pairs by their key in reverse order.used for sorting.
 * @param lhs The first pair.
 * @param rhs The second pair.
 * @return True if the key of the first pair is greater than the
 * key of the second pair, false otherwise.
 */
bool compare_pairs_by_key_reverse (const t_pair& lhs, const t_pair& rhs)
{
  return lhs.first > rhs.first;
}

/**
 * Returns a map containing the k best values by key from a vector of pairs.
 * @param vec The input vector of pairs.
 * @param k The number of best values to return.
 * @return A map containing the k best values by key.
 */
std::map<double,sp_movie>
    get_k_best_values_by_key (std::vector<t_pair> vec,int k)
{
  std::sort(vec.begin(), vec.end(),compare_pairs_by_key_reverse);
  std::map<double,sp_movie> my_map;
  for(int i=0;i<k;i++)
  {
    my_map[vec[i].first] = vec[i].second;
  }
  return my_map;
}

/**
 * Calculates the sum of the keys in a map.
 * @param temp_rank_map The input map.
 * @return The sum of the keys in the map.
 */
double summing_map(const std::map<double,sp_movie>& temp_rank_map)
{
  double sum = 0;
  for (const auto &it: temp_rank_map)
  {
      sum += it.first;
  }
  return sum;
}


sp_movie RecommendationSystem::recommend_by_cf(const User& user, int k)
{
  rank_map temp_rank_map = user.get_ranks();
  sp_movie res_movie;
  double max_res =0;
  for (auto &it: main_ranking_map)
  {
    if(temp_rank_map.find(it.first) == temp_rank_map.end())
    {
      double semi_res = predict_movie_score (user,it.first,k);
      if(semi_res>max_res)
      {
        max_res = semi_res;
        res_movie = it.first;
      }
    }
  }
  return res_movie;
}


double RecommendationSystem::predict_movie_score(const User &user, const
sp_movie &movie,int k)
{
  rank_map temp_rank_map = user.get_ranks();
  std::vector<t_pair> k_vec;
  for (auto &pair: temp_rank_map)
  {
    k_vec.emplace_back(ang_calc(main_ranking_map[movie],
                               main_ranking_map[pair.first]),pair.first);
  }
  auto k_best_movies = get_k_best_values_by_key(k_vec, k);
  double sum = summing_map(k_best_movies);
  double semi_res=0;
  for(auto &pair: k_best_movies)
  {
    semi_res+= (pair.first * temp_rank_map[pair.second]);
  }
  return semi_res/sum;
}

sp_movie RecommendationSystem::get_movie(const std::string &name, int year)
const
{
  sp_movie new_movie_ptr = std::make_shared<Movie>(name, year);
  if(main_ranking_map.find (new_movie_ptr) != main_ranking_map.end())
  {
    return main_ranking_map.find (new_movie_ptr)->first;
  }
  return nullptr;
}

/**
 * Overloaded insertion operator to print the movies in the
 * recommendation system.
 * @param os The output stream.
 * @param rec_system The recommendation system object.
 * @return The output stream with the movies printed.
 */
std::ostream & operator<<(std::ostream& os, RecommendationSystem& rec_system)
{
  for (auto &pair: rec_system.main_ranking_map)
  {
    os<<*(pair.first);
  }
  return os;
}