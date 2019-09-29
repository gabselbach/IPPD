#include <omp.h>
#include <vector>
#include <iostream>
using namespace std;
 
vector<long> merge(const vector<long>& left, const vector<long>& right)
{
    vector<long> result;
    unsigned left_it = 0, right_it = 0;
 
    while(left_it < left.size() && right_it < right.size())
    {
        if(left[left_it] < right[right_it])
        {
            result.push_back(left[left_it]);
            left_it++;
        }
        else                    
        {
            result.push_back(right[right_it]);
            right_it++;
        }
    }
 
    // Push the remaining data from both vectors onto the resultant
    while(left_it < left.size())
    {
        result.push_back(left[left_it]);
        left_it++;
    }
 
    while(right_it < right.size())
    {
        result.push_back(right[right_it]);
        right_it++;
    }
 
    return result;
}
 
vector<long> mergesort(vector<long>& vec, int threads)
{
    // Termination condition: List is completely sorted if it
    // only contains a single element.
    if(vec.size() == 1)
    {
        return vec;
    }
 
    // Determine the location of the middle element in the vector
    std::vector<long>::iterator middle = vec.begin() + (vec.size() / 2);
 
    vector<long> left(vec.begin(), middle);
    vector<long> right(middle, vec.end());
 
    // Perform a merge sort on the two smaller vectors
 
    if (threads > 1)
    {
      #pragma omp parallel sections
      {
        #pragma omp section
        {
          left = mergesort(left, threads/2);
        }
        #pragma omp section
        {
          right = mergesort(right, threads - threads/2);
        }
      }
    }
    else
    {
      left = mergesort(left, 1);
      right = mergesort(right, 1);
    }
 
    return merge(left, right);
}
 
int main()
{
    vector<long> v(10);
    for (long i=0; i<10; ++i)
        v[i] = (i * i) % 10;
    clock_t inicio = clock();
    v = mergesort(v, 1);
    printf("valor do tempo e %.4f\n",(clock()-inicio)*1000 / (double)CLOCKS_PER_SEC);
}