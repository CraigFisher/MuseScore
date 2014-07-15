//=============================================================================
//  MuseScore
//  Music Composition & Notation
//  $Id:$
//
//  Copyright (C) 2012 Werner Schweer
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License version 2
//  as published by the Free Software Foundation and appearing in
//  the file LICENCE.GPL
//=============================================================================

#include <QtTest/QtTest>

#include "libmscore/score.h"
#include "libmscore/notemappings.h"
#include "libmscore/stafftype.h"
#include "mtest/testutils.h"

#define DIR QString("libmscore/altnoteplacement/")

using namespace Ms;

//---------------------------------------------------------
//   TestAltNotePlacement
//---------------------------------------------------------

class TestAltNotePlacement : public QObject, public MTest
      {
      Q_OBJECT

      void testTpcChoice(NoteMappings* n, int tpcSet, int tpcExpected);

   private slots:
      void initTestCase();
      void tpcPlacements1();
      };

//---------------------------------------------------------
//   initTestCase
//---------------------------------------------------------

void TestAltNotePlacement::initTestCase()
      {
      initMTest();
      }

//---------------------------------------------------------
//   tpcPlacements1
//          Test choices made by function getTpc(int position)
//                (without any key signature awareness).
//          getTpc() is biased to choose tpc's in this order:
//                1 naturals
//                2 flats
//                3 sharps
//                4 double flats
//                5 double sharps
//---------------------------------------------------------

void TestAltNotePlacement::tpcPlacements1()
      {
      StaffTypeTemplate* st = new StaffTypeTemplate(root + "/" + DIR + "chromatic1.stt");
      NoteMappings* n = st->noteMappings();
      
      //double flats
      testTpcChoice(n, -1, 11);
      testTpcChoice(n, 0, 12);
      testTpcChoice(n, 1, 13);
      testTpcChoice(n, 2, 14);
      testTpcChoice(n, 3, 15);
      testTpcChoice(n, 4, 16);
      testTpcChoice(n, 5, 17);
      
      //double sharps
      testTpcChoice(n, 27, 15);
      testTpcChoice(n, 28, 16);
      testTpcChoice(n, 29, 17);
      testTpcChoice(n, 30, 18);
      testTpcChoice(n, 31, 19);
      testTpcChoice(n, 32, 8);
      testTpcChoice(n, 33, 9);
      
      //sharps
      testTpcChoice(n, 20, 8);
      testTpcChoice(n, 21, 9);
      testTpcChoice(n, 22, 10);
      testTpcChoice(n, 23, 11);
      testTpcChoice(n, 24, 12);
      testTpcChoice(n, 25, 13);
      testTpcChoice(n, 26, 14);
      
      //flats
      testTpcChoice(n, 13, 13);
      testTpcChoice(n, 14, 14);
      testTpcChoice(n, 15, 15);
      testTpcChoice(n, 16, 16);
      testTpcChoice(n, 17, 17);
      testTpcChoice(n, 18, 18);
      testTpcChoice(n, 19, 19);
      
      //naturals
      testTpcChoice(n, 6, 18);
      testTpcChoice(n, 7, 19);
      testTpcChoice(n, 8, 8);
      testTpcChoice(n, 9, 9);
      testTpcChoice(n, 10, 10);
      testTpcChoice(n, 11, 11);
      testTpcChoice(n, 12, 12);

      //TODO: test adjusted tpc's
      AccidentalVal val = AccidentalVal::NATURAL;
      }



void TestAltNotePlacement::testTpcChoice(NoteMappings* n, int tpcSet, int tpcExpected) {
      int position = n->tpc2Position(tpcSet);
      int tpc = n->getTpc(position);
      QCOMPARE(tpc, tpcExpected);
      }

QTEST_MAIN(TestAltNotePlacement)

#include "tst_altnoteplacement.moc"

