/**
 * @file lyrics.h
 *
 * Handling signal frames.
 *
 * This file is part of the ispeech library.
 * ispeech is free software, licensed under the MIT/X11 License. A copy of
 * the license is provided with the library in the LICENSE file.
 *
 * @package vfx_ispeech
 * @version 0.0.1-dev
 * @author Han Zhongtao
 * @date 2019-2019
 * @license
 * @since 0.0.1
 */

#ifndef LYRICS_H
#define LYRICS_H



#include <iostream>
#include <fstream>
using namespace std;
/*
**简单歌词分析:
**根据开始时间和结束时间 来显示
*/

void readLyric(string fileName);
void analysisOneLine(string str);
void printLyric();
void getLyricHeader(string str);
int  changeStringToInt(string str_time);
void timeSort();

const int MAX_LYRIC_LINE_NUM =200;
const int MAX_LYRCE_REPEAT_NUM = 10;
string header[] = {"ai:", "ar:", "al:", "by:"};
int lineNum = 0;
string Lheader[4];

struct OneLineLyric
{
  int lineNum;
  string startTime;
  string endTime;
  string lyricContent;
} lyric[MAX_LYRIC_LINE_NUM ]; //动态分配最好


void readLyric(string fileName)
{
  ifstream fin;
  fin.open(fileName);
  if(!fin.is_open())
    {
      cerr << "文件读取失败!\n";
      exit(0);
    }
  string str;
  getline(fin, str);
  while(fin)
    {
      if(str != "")
	{
	  analysisOneLine(str);
	}
      getline(fin, str);
    }

  fin.close();
  printLyric();
}

//分析一行 保存到结构体数组
void analysisOneLine(string str)
{
  if(str[0] != '[')
    {
      cout << "歌词文件格式不正规!\n";
    }

  //歌词说明 ti:歌曲名称 ar:演唱者 al: by: 制作单位
  getLyricHeader(str);

  int m, n, p = 0;
  string timeTemp[MAX_LYRCE_REPEAT_NUM];//保存信息
  m = str.find_first_of('[');
  n = str.find_first_of(']');
  while(m >= 0 && m <= str.length() && n >= 0 && n <= str.length() && str != "")
    {
      timeTemp[p] = str.substr(m + 1, n - 1);
      p++;
      str = str.substr(n + 1, str.length());
      m = str.find_first_of('[');
      n = str.find_first_of(']');
    }

  string contentTemp = str;

  for(int i = 0; i < p; i++)
    {
      if(lineNum == 1)
	{
	  cout << endl;
	}
      lyric[lineNum].startTime = timeTemp[i];

      if(timeTemp[i + 1] != "") //连续的
	{
	  lyric[lineNum].endTime = timeTemp[i + 1];
	  cout << timeTemp[i] << endl;
	}

      if(lineNum - 1 >= 0 && i == 0) //设置上一个的endTime
	lyric[lineNum - 1].endTime = lyric[lineNum].startTime;
      lyric[lineNum].lineNum = lineNum;
      lyric[lineNum].lyricContent = contentTemp;
      lineNum++;
    }

}

void getLyricHeader(string str)
{
  for(int i = 0; i < 4; i++)
    {
      if(header[i] == str.substr(1, 3))
	{
	  Lheader[i] = str;
	}
    }
}

//显示数据
void printLyric()
{
  cout << "歌曲信息:" << endl;
  for(int i = 0; i < 4; i++)
    {
      cout << Lheader[i] << endl;
    }

  for(int i = 0; i < lineNum; i++)
    {
      if(lyric[i].startTime != "")
	{
	  cout << "行    号:\t" << lyric[i].lineNum << endl;
	  cout << "开始时间:\t" << lyric[i].startTime << endl;
	  cout << "开始时间:\t" << changeStringToInt(lyric[i].startTime) << endl;


	  cout << "结束时间:\t" << lyric[i].endTime << endl;
	  cout << "歌词内容:\t" << lyric[i].lyricContent << endl;
	  cout << endl;
	}
    }
}

//将时间转换成毫秒
int changeStringToInt(string str_time)
{
  int time, min, sec, msc = 0; //分钟,秒,毫秒 转换成毫秒
  min = atof(const_cast<const char *>(str_time.substr(0, 2).c_str()));
  sec = atof(const_cast<const char *>(str_time.substr(3, 2).c_str()));
  if(str_time.length() > 5)
    msc = atof(const_cast<const char *>(str_time.substr(4, 2).c_str()));
  time = min * 60 * 1000 + sec * 1000 + msc;
  return time;
}

#endif //LYRICS_H
