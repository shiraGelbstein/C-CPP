

// don't change those includes
#include "User.h"
#include "RecommendationSystem.h"


// implement your cpp code here
User::User(string & name, rank_map & ranking_map,
           std::shared_ptr<RecommendationSystem> rec_system) :
user_name(name), ranking_map(ranking_map),rec_system(std::move(rec_system))
{}

void User::add_movie_to_rs(const std::string &name, int year,
                     const std::vector<double> &features,
                     double rate)
{
  sp_movie new_movie = rec_system->add_movie(name,year,features);
  ranking_map[new_movie] = rate;
}

sp_movie User::get_recommendation_by_content() const
{
  sp_movie recommended_movie = rec_system->recommend_by_content (*this);
  return recommended_movie;
}

sp_movie User::get_recommendation_by_cf(int k) const
{
  sp_movie recommended_movie = rec_system->recommend_by_cf(*this,k);
  return recommended_movie;
}

double User::get_prediction_score_for_movie(const std::string& name, int year,
                                       int k) const
{
  sp_movie new_sp_movie = rec_system->get_movie(name, year);
  double predicted_score = rec_system->predict_movie_score(*this,
                                                             new_sp_movie,k);
  return predicted_score;
}

std::ostream & operator<<(std::ostream& os, User& user)
{
  std::cout<<"name: "<<user.get_name()<<std::endl;

  std::cout<<*(user.rec_system);

  std::cout<<"\n";
  return os;
}