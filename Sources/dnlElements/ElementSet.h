/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  By Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2019                                                *
 *                                                                         *
 **************************************************************************/

/*!
  \file NodeSet.h
  \brief Declaration file for the NodeSet class

  This file is the declaration file for the NodeSet class.

  \ingroup dnlFEM
  \author &copy; Olivier PANTALE
  \date 1997-2019
*/

#ifndef __dnlElements_ElementSet_h__
#define __dnlElements_ElementSet_h__

//#include <upFemSolver.h>

/*!
  \file FiniteSet.h
  \brief fichier .h de definition des groupes de noeuds et elements
  \ingroup femLibrary

  Ce fichier sert e la definition des groupes de noeuds et elements de la structure.

  \author &copy; Olivier PANTALE
  \version 0.9.6
  \date 1997-2004
*/

/* #include <upFemSolver.h> */

#include <String.h>
#include <List.h>
#include <Element.h>

/*!
  \brief Classe de gestion des groupes d'elements
  \ingroup femLibrary
  \class ElementSet FiniteSet.h

 Cette classe gere les groupes d'elements au niveau d'une structure sur DynELA. Elle comporte des methodes de creation de groupes d'elements (par ajout d'elements generalement), ainsi que des methodes de gestion de ces groupes d'elements.

  \author &copy; Olivier PANTALE
  \version 0.9.6
  \date 1997-2004
*/
class ElementSet
{
public:
  String name;                   //!< Nom public de l'ElementSet
  List<Element *> elements; //!< Liste des elements

public:
  // constructeurs
  ElementSet(char *newName = NULL);
  ElementSet(const ElementSet &X);
  ~ElementSet();

  void add(Element *element);
  void add(ElementSet *elementSet);
  long getSize();
  void setName(char *_name);
  Element *getElement(long item);
  long getNumber(long item);
  void intersect(ElementSet *elementSet);
  void substract(ElementSet *elementSet);
  void clear();
};

#endif
