//
// Created on 2/20/2022.
//

#ifndef RECOMMENDATIONSYSTEM_H
#define RECOMMENDATIONSYSTEM_H
#include <map>
#include "User.h"

struct comp_func_sp
{
  bool operator()(const sp_movie &sp_movie_1,const sp_movie& sp_movie_2)
  const
  {
    return (*sp_movie_1) < (*sp_movie_2);
  }
};


class RecommendationSystem
{
 private:
  std::map<sp_movie ,std::vector<double>, comp_func_sp> main_ranking_map;
  friend double ang_calc(const std::vector<double>& vec1, const
  std::vector<double>& vec2);
  friend double summing_map(const std::map<double,sp_movie>& temp_rank_map);

public:

	RecommendationSystem() = default;
    /**
     * adds a new movie to the system
     * @param name name of movie
     * @param year year it was made
     * @param features features for movie
     * @return shared pointer for movie in system
     */
	sp_movie add_movie
    (const std::string& name,int year,const std::vector<double>& features);


    /**
     * a function that calculates the movie with
     * highest score based on movie features
     * @param ranks user ranking to use for algorithm
     * @return shared pointer to movie in system
     */
	sp_movie recommend_by_content(const User& user);

    /**
     * a function that calculates the movie with highest
     * predicted score based on ranking of other movies
     * @param ranks user ranking to use for algorithm
     * @param k
     * @return shared pointer to movie in system
     */
	sp_movie recommend_by_cf(const User& user, int k);


    /**
     * Predict a user rating for a movie given argument using item
     * cf procedure with k most similar movies.
     * @param user_rankings: ranking to use
     * @param movie: movie to predict
     * @param k:
     * @return score based on algorithm as described in pdf
     */
	double predict_movie_score(const User &user, const sp_movie &movie,
												  int k);

	/**
	 * gets a shared pointer to movie in system
	 * @param name name of movie
	 * @param year year movie was made
	 * @return shared pointer to movie in system
	 */
	sp_movie get_movie(const std::string &name, int year) const;



    friend std::ostream & operator<<(std::ostream& os, RecommendationSystem&
    rec_system);
};


#endif //RECOMMENDATIONSYSTEM_H
