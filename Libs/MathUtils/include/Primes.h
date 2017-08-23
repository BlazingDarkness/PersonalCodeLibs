#pragma once
#include <bitset>
#include <list>

namespace MathUtils
{
	namespace Numbers
	{
		template<size_t tableSize>
		std::bitset<tableSize>* GeneratePrimeTable()
		{
			std::bitset<tableSize>* table = new std::bitset<tableSize>();
			table->set();
			(*table)[0] = false;
			(*table)[1] = false;

			//Sieve out all non-primes
			for (size_t i = 2; i < tableSize; ++i)
			{
				if ((*table)[i])
				{
					for (size_t j = 2 * i; j < tableSize; j += i)
					{
						(*table)[j] = false;
					}
				}
			}

			return table;
		}

		template<size_t numberRange, typename NumberType = int>
		std::list<NumberType>* GeneratePrimeList()
		{
			std::bitset<numberRange>* table = new std::bitset<numberRange>();
			table->set();
			(*table)[0] = false;
			(*table)[1] = false;

			//Sieve out all non-primes
			for (size_t i = 2; i < numberRange; ++i)
			{
				if ((*table)[i])
				{
					for (size_t j = 2 * i; j < numberRange; j += i)
					{
						(*table)[j] = false;
					}
				}
			}

			std::list<NumberType>* primeList = new std::list<NumberType>;

			//Compile prime list
			for (size_t i = 0; i < numberRange; ++i)
			{
				if ((*table)[i])
				{
					primeList->push_back(static_cast<NumberType>(i));
				}
			}

			delete table;

			return primeList;
		}

		//Brute force check if a number is prime
		template<typename NumberType = int>
		bool CheckIfPrime(NumberType n)
		{
			//Start at first odd prime
			NumberType divisor = static_cast<NumberType>(3);

			//Check if even
			if (n % 2 == 0)
				return false;

			//Check all odd divisors below square root
			while (divisor * divisor < n)
			{
				if (n % divisor == 0)
					return false;
				else
					divisor += 2; //skip even divisors
			}

			//Square number check
			if (divisor * divisor == n)
				return false;
			else
				return true;
		}

		//Uses a pre calculated prime list so that it only needs to check prime divisors
		template<typename NumberType = int>
		bool CheckIfPrime(NumberType n, std::list<NumberType>* primeList) {
			auto divItr = primeList->begin();

			//Check if even
			if (n % 2 == 0)
				return false;

			//Check if primeList contains all primes below the sqrt of n
			if ((divItr != primeList->end()) &&
				(primeList->back() * primeList->back() >= n))
			{
				//Check all prime divisors below square root
				while ((*divItr) * (*divItr) < n)
				{
					if (n % divisor == 0)
						return false;
					else
						++divItr;
				}
			}
			else
			{
				//Check all prime divisors within prime list
				while (divItr != primeList->end())
				{
					if (n % divisor == 0)
						return false;
					else
						++divItr;
				}

				//Start at first odd number after last prime in list
				NumberType divisor = static_cast<NumberType>(3);

				//Check all odd divisors below square root
				while (divisor * divisor < n)
				{
					if (n % divisor == 0)
						return false;
					else
						divisor += 2; //skip even divisors
				}
			}

			//Square number check
			if (divisor * divisor == n)
				return false;
			else
				return true;
		}
	}
}