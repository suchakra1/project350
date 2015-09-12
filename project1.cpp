/* project1.cpp 
Program for face recognition as required in image recognition project  
Subhendu Chakrabarti v1.3 , 2014
To  compile link boost  lib  boost_iostreams
data files(txt)= jpg image in int array, three query as three 


Output: fast data IO using boost

*/



#include <algorithm>
#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <cstring>
#include <vector>
#include <list>
#include <iomanip>
#include <sstream>
#include <utility> 
#include <cmath>
#include <assert.h>
#include <time.h>
#include <sys/resource.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <streambuf>
#include <istream>
#include <sys/mman.h>

#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/range/numeric.hpp>

namespace io = boost::iostreams;
namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

using namespace boost::iostreams;
using std::ifstream;
using std::istringstream;
using std::list;
using std::istream_iterator;
using std::back_insert_iterator;
using std::vector;
using std::string;
using std::cout;


double tstart;



//Function for reading  file (.dat) data

void use_boost_spirit(const std::string& data, std::vector<double>& result)
{
    qi::phrase_parse(data.begin(), data.end(), *qi::double_, qi::space, result);
}



int main(){

  char fn_qdata[32];
  char fn_qdata_t[32];
  char tmp_int_q[3][32];
  char tmp_int_t[32]="_template.dat";

  strcpy(tmp_int_q[0],"_AU01_query.dat");
  strcpy(tmp_int_q[1],"_AU12_query.dat");
  strcpy(tmp_int_q[2],"_AU17_query.dat");

  tstart = clock();
  
  std::vector<double> imvector;
  std::vector<double> tempvector;
  list<std::pair<double,int> >  qDataList_pair;

  int  k;
  double s;
  double xsqr;
  double ysqr;


 //Reading files Xi=imvector=image file Yj=tempvector=template file
 //start 47 template file  loopYj

     for(int i=1;i<=47;i++){
       
     
     sprintf(fn_qdata_t, "%03d", i);     
     strcat(fn_qdata_t, tmp_int_t);   
     
     boost::iostreams::mapped_file_source file;
     file.open(fn_qdata_t);
     std::string str = file.data();
     
    tempvector.clear();
    tempvector.reserve(777216);
   
     use_boost_spirit(str,tempvector);
  
  
     //start query file loop Xi
     for(int j=0;j<3;j++){

       sprintf(fn_qdata, "%03d", i);     
      strcat(fn_qdata, tmp_int_q[j]);  
       
     boost::iostreams::mapped_file_source imfile;
     imfile.open(fn_qdata);
     std::string imstr = imfile.data();
     
    imvector.clear();
    imvector.reserve(5632);
     use_boost_spirit(imstr,imvector);

  k=0;
  s=0.;
  xsqr=0;
  ysqr=0;

   std::vector<double>::iterator it = imvector.begin(); 
  

  //loop over templates every 5632 iterator one Yj

  for (std::vector<double>::iterator itx = tempvector.begin() ; itx != tempvector.end(); ++itx){


    //calculation of sim. function
    s+=(*it)*(*itx);
    xsqr+=pow((*it),2);
     ysqr+=pow(*(itx),2);

    ++it;
    k++;

    if(k%5632==0){      
      //re-initialize every template and save sim value into numbered list
   s=s/(sqrt(xsqr)*sqrt(ysqr));
    qDataList_pair.push_back(std::make_pair(s,int(k/5632)));

    s=0.;
    xsqr=0.;
    ysqr=0;
        it = imvector.begin();

    }

  }//end query it loop

   //Select 10NN values from the sim- list using stl

    qDataList_pair.sort();
    qDataList_pair.reverse();

    int m=0;
    std::cout<<"File: "<<fn_qdata<<" NN:";
  
    for (list<std::pair<double,int> >::iterator itn = qDataList_pair.begin() ; m<10 ; itn++){std::cout<<(*itn).second<<" ";m++;}//display list loop 
    std::cout<<"\n"<<"Sim: ";
     m=0;
     for (list<std::pair<double,int> >::iterator itn = qDataList_pair.begin() ; m<10 ; itn++){std::cout<<(*itn).first<<" ";m++;}//display list loop 
     std::cout<<"\n";

      qDataList_pair.clear();
      imvector.clear();
     }//end queary file loop


     }//end template loop
 
 tstart = clock()-tstart;


  fprintf(stdout, "The total CPU time is: %f seconds \n\n", tstart/CLOCKS_PER_SEC);

}

  





