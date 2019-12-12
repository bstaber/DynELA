/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  By Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2019                                                *
 *                                                                         *
 **************************************************************************/

/*!
  \file Parallel.h
  \brief Declaration file for the Parallel class

  This file is the declaration file for the Parallel class.

  \ingroup dnlFEM
  \author &copy; Olivier PANTALE
  \date 1997-2019
*/

#ifndef __dnlFEM_Parallel_h__
#define __dnlFEM_Parallel_h__

#include <String.h>
#include <List.h>
#include <omp.h>

class Element;

/* #include <iostream>
#include <fstream>
 */
class ElementsChunk
{
public:
  List<Element *> elements; //!< List of the elements of the chunk

public:
  ElementsChunk() {}
  ~ElementsChunk() {}
};

class Parallel
{

private:
  int _cores = 1;
  int _maxThreads = 1;
  ElementsChunk **_elementsChunks; //!< Elements chunks

private:
ElementsChunk ** _initChunkList();
void _deleteChunkList(ElementsChunk** chunkList);

public:
  String name = "_noname_";

  // constructeurs
  Parallel(char *newName = NULL);
  ~Parallel();

  ElementsChunk *getElementsOfCore(int core);
  ElementsChunk *getElementsOfCurrentCore();
  int getCores();
  void dispatchElements(List<Element *> elementList);
  void setCores(int cores);
};

//-----------------------------------------------------------------------------
inline int Parallel::getCores()
//-----------------------------------------------------------------------------
{
  return _cores;
}

//-----------------------------------------------------------------------------
inline ElementsChunk *Parallel::getElementsOfCore(int core)
//-----------------------------------------------------------------------------
{
  return _elementsChunks[core];
}

//-----------------------------------------------------------------------------
inline ElementsChunk *Parallel::getElementsOfCurrentCore()
//-----------------------------------------------------------------------------
{
  return _elementsChunks[omp_get_thread_num()];
}

#endif
