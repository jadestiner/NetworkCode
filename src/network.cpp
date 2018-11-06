#include "network.h"
#include "random.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <random>



/*! 
     Resizes the list of nodes (\ref values) and also resets all values.
     After this function is called \ref values has size *n* and contains random numbers (normal distribution, mean=0, sd=1).
  */
void Network::resize(const size_t& size)
{
	values.clear();
	values.resize(size);
	RNG.normal(values,0,1);	
}

/*! Adds a bidirectional link between two nodes
  @param[in] a,b the indexes if the two nodes 
  @param[out] success the link was succesfully inserted,
  @param[out] success the link was succesfully inserted (true if both nodes exist and the link did not exist yet)
 */
bool Network::add_link(const size_t& num1, const size_t& num2)
{
	std::vector<size_t> v(neighbors(num1));
	if(std::find(v.begin(), v.end(), num2) != v.end()) 
	{
		return false;
	} else if((num1!= num2) and (num2<values.size()) and (num1<values.size()))
		{
		links.insert({num1, num2});
		links.insert({num2, num1});
		return true;
		}
		else {return false;}
}

/*! Creates random connections between nodes: each node *n* will be linked with *degree(n)* other nodes (randomly chosen) where *degree(n)* is Poisson-distributed.

  All previous links are erased first.
  @param[out] the number of links created.
  @param mean_deg the average of the Poisson distribution.
 */
size_t Network::random_connect(const double& n)
{
	int compteur(0);
	std::multimap<size_t, size_t>::iterator iter;
	for(iter=links.begin(); iter!=links.end(); ++iter)
	{
		links.erase(iter);
	}
	
	for (int it = 0; it<values.size(); ++it)
	{
		int degree(RNG.poisson(n));
		std::vector<int> v(degree);
		RNG.uniform_int(v,0,values.size()-1);
		for(int i=0; i<v.size(); ++i )
		{
			if(add_link(it,v[i]))
			{++compteur;}
		}
	}
	return compteur;
}


/*! Resets node values with a vector of *n* new values.
   If the current size is *s* and *n<s* then only the first *n* values are changed.
   If *n>s* then only *s* values are used. 
   The internal \ref values vector keeps the same size.

   @param[in] vector of new node values,
   @param[out] number of nodes succesfully reset.
  @param[in] vector of new node values
  @param[out] number of nodes succesfully reset
 */
size_t Network::set_values(const std::vector<double>& newvalues)
{
	if(values.size()>newvalues.size())
	{
		for(size_t i=0; i<newvalues.size();++i)
		{
			values[i]=newvalues[i];
		}
	}
	if(values.size()<newvalues.size())
	{
		for(size_t i=0; i<values.size();++i)
		{
			values[i]=newvalues[i];
		}
	}else{values=newvalues;}
	
	return values.size();
}

/*! Number of nodes */ 
size_t Network::size() const
{
	return values.size();
}

/*! Degree (number of links) of node no *n* */
size_t Network::degree(const size_t &_n) const
{  
       return links.count(_n);
}

/*! Value of node no *n* */
double Network::value(const size_t &_n) const
{
	return values[_n];
}

/*! All node values in descending order 
 \ref values is left unchanged by this operation*/
std::vector<double> Network::sorted_values() const
{
	std::vector<double> sortvals(values);
	std::sort(sortvals.begin(), sortvals.end(), std::greater<double>());
	return sortvals;
}

/*! All neighbors (linked) nodes of node no *n* */
std::vector<size_t> Network::neighbors(const size_t& n) const
{
	std::pair<std::multimap<size_t, size_t>::const_iterator, std::multimap<size_t, size_t>::const_iterator> iter;
    iter = links.equal_range(n);
    std::vector<size_t> voisins;
    while (iter.first != iter.second)
    {
		voisins.push_back(iter.first->second);
        iter.first++;
    }
return voisins;
	
}
