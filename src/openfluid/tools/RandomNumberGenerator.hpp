/*

  This file is part of OpenFLUID software
  Copyright(c) 2007, INRA - Montpellier SupAgro


 == GNU General Public License Usage ==

  OpenFLUID is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OpenFLUID is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OpenFLUID. If not, see <http://www.gnu.org/licenses/>.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
  
*/


/**
  @file RandomNumberGenerator.hpp

  @author Dorian GERARDIN <dorian.gerardin@inrae.fr>
  @author Alain DUPUY <contact@openfluid-project.org>
  @author David CREVOISIER <david.crevoisier@inrae.fr>
 */


#ifndef __OPENFLUID_TOOLS_RANDOMNUMBERGENERATOR_HPP__
#define __OPENFLUID_TOOLS_RANDOMNUMBERGENERATOR_HPP__


#include <string>
#include <iostream>
#include <random>
#include <vector>

#include <openfluid/ware/TypeDefs.hpp>


namespace openfluid { namespace tools {


class OPENFLUID_API RandomNumberGenerator
{
  public:

    RandomNumberGenerator();

    ~RandomNumberGenerator() {}

    void init(long int CustomSeed = -1);

    uint64_t getSelectedSeed() const;

    /**
    * @brief Log normal distribution
    *
    * @tparam T type of value for distribution
    * @param N number of values to generate with distribution
    * @param Mean mean for log normal distribution
    * @param Sd standard deviation for log normal distribution
    * @return std::vector<T> vector with N randomized values using log normal distribution
    */
    template <typename T>
    std::vector<T> rlnorm(size_t N, double Mean, double Sd)
    {
      T Maxlnorm = std::exp(Mean - Sd * Sd);
      std::vector<T> vec(N, Maxlnorm);
      std::lognormal_distribution<T> Distribution(Mean, Sd);

      for (auto& v : vec)
      {
        v = Distribution(getGenerator());
      }

      return vec;
    }


    // =====================================================================
    // =====================================================================


    /**
    * @brief Normal distribution
    *
    * @tparam T type of value for distribution
    * @param N number of values to generate with distribution
    * @param Mean mean for normal distribution
    * @param Sd standard deviation for normal distribution
    * @return std::vector<T> vector with N randomized values using normal distribution
    */
    template <typename T>
    std::vector<T> rnorm(size_t N, double Mean, double Sd)
    {
      std::vector<T> vec(N, Mean + Sd);
      std::normal_distribution<T> Distribution(Mean, Sd);
      for (auto& v : vec)
      {
        v = Distribution(getGenerator());
      }

      return vec;
    }


    // =====================================================================
    // =====================================================================


    /**
    * @brief Uniform distribution
    *
    * @tparam T type of value for distribution
    * @param N number of values to generate with distribution
    * @param V1 min value for distribution
    * @param V2 max value for distribution
    * @return std::vector<T> vector with N randomized values using real uniform distribution
    */
    template <typename T>
    std::vector<T> runif(size_t N, T V1, T V2)
    {
      std::vector<T> vec(N, 0.5 * V1 + 0.5 * V2);
      if (V2 > V1)
      {
        std::uniform_real_distribution<T> Distribution(V1, V2);
        for (auto& v : vec)
        {
          v = Distribution(getGenerator());
        }
      }
      else 
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Max value < Min Value");
      }

      return vec;
    }


    // =====================================================================
    // =====================================================================


    /**
    * @brief Uniform distribution
    *
    * @tparam T type of value for distribution
    * @param N number of values to generate with distribution
    * @param V1 min value for distribution
    * @param V2 max value for distribution
    * @return std::vector<T> vector with N randomized values using integer uniform distribution
    */
    template <typename T>
    std::vector<T> irunif(size_t N, T V1, T V2)
    {
      std::vector<T> vec(N, 0.5 * V1 + 0.5 * V2);
      if (V2 > V1)
      {
        std::uniform_int_distribution<T> Distribution(V1, V2);
        for (auto& v : vec)
        {
          v = Distribution(getGenerator());
        }
      }
      else 
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Max value < Min Value");
      }

      return vec;
    }


    // =====================================================================
    // =====================================================================


    /**
    * @brief Log normal distribution
    *
    * @tparam T type of value for distribution
    * @param Mean mean for log normal distribution
    * @param Sd standard deviation for log normal distribution
    * @return Randomized value using log normal distribution
    */
    template <typename T>
    T rlnorm(double Mean, double Sd)
    {
      std::lognormal_distribution<T> Distribution(Mean, Sd);
      return Distribution(getGenerator());
    }


    // =====================================================================
    // =====================================================================


    /**
    * @brief Normal distribution
    *
    * @tparam T type of value for distribution
    * @param Mean mean for normal distribution
    * @param Sd standard deviation for normal distribution
    * @return Randomized value using normal distribution
    */
    template <typename T>
    T rnorm(double Mean, double Sd)
    {
      std::normal_distribution<T> Distribution(Mean, Sd);
      return Distribution(getGenerator());
    }


    // =====================================================================
    // =====================================================================


    /**
    * @brief Real uniform distribution
    *
    * @tparam T type of value for distribution
    * @param V1 min value for distribution
    * @param V2 max value for distribution
    * @return Randomized value using real uniform distribution
    */
    template <typename T>
    T runif(T V1, T V2)
    {
      if (V2 > V1)
      {
        std::uniform_real_distribution<T> Distribution(V1, V2);
        return Distribution(getGenerator());
      }
      else 
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Max value < Min Value");
      }
    }


    // =====================================================================
    // =====================================================================


    /**
    * @brief Integer uniform distribution
    *
    * @tparam T type of value for distribution
    * @param V1 min value for distribution
    * @param V2 max value for distribution
    * @return Randomized value using integer uniform distribution
    */
    template <typename T>
    T irunif(T V1, T V2)
    {
      if (V2 > V1)
      {
        std::uniform_int_distribution<T> Distribution(V1, V2);
        return Distribution(getGenerator());
      }
      else 
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, "Max value < Min Value");
      }
    }


    // =====================================================================
    // =====================================================================


    /**
    * @brief Bernoulli distribution
    *
    * @param Probability probability of success
    * @return Randomized value using bernoulli distribution
    */
    bool bernoulli(double Probability)
    {
      std::bernoulli_distribution Distribution(Probability);
      return Distribution(getGenerator());
    }

  
  protected:
  
    std::mt19937_64 m_Generator;

    uint64_t m_SelectedSeed;

    bool m_DisplaySeed;

    const std::string m_DisplayNameInfo;

    std::mt19937_64& getGenerator();

    virtual void displaySeedInfo();

};


} } // namespaces

#endif /* __OPENFLUID_TOOLS_RANDOMNUMBERGENERATOR_HPP__ */
