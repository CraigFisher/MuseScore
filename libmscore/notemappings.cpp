//=============================================================================
//  MuseScore
//  Music Composition & Notation
//
//  Copyright (C) 2002-2011 Werner Schweer
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License version 2
//  as published by the Free Software Foundation and appearing in
//  the file LICENCE.GPL
//=============================================================================

#include "NoteMappings.h"
#include "xml.h"

//cc
namespace Ms {

//---------------------------------------------------------
//   NoteMappings
//---------------------------------------------------------

NoteMappings::NoteMappings(QFile* f)
      {
      if (!(f->open(QIODevice::ReadOnly)))
            throw (QObject::tr("failed to open."));
      
      XmlReader e(f);
      while (e.readNextStartElement()) {
            if (e.name().toString() == "noteMappings")
                  read(e);
            else
                  throw (QObject::tr("improper format."));
            }
      }
      
void NoteMappings::write(Xml& xml) const
      {
      xml.stag(QString("noteMappings"));

      if (!(_notePositions.empty() && _noteHeads.empty()))
            writeMappings(xml);
      if (!_clefOffsets.empty())
            writeClefOffsets(xml);
      xml.tag("octaveDistance", _octaveDistance);
      xml.tag("noAccidentals", (int)_noAccidentals);
      
      xml.etag();
      }
      
void NoteMappings::read(XmlReader& e)
      {
      while (e.readNextStartElement()) {
            QString tag = e.name().toString();
            if (tag == "mappings")
                  readMappings(e);
            else if (tag == "clefOffsets")
                  readClefOffsets(e);
            else if (tag == "octaveDistance")
                  _octaveDistance = e.readInt();
            else if (tag == "noAccidentals")
                  _noAccidentals = e.readBool();
            else
                  throw (QObject::tr("improperly formated."));
            }
      }

void NoteMappings::writeMappings(Xml& xml) const
      {
      xml.stag("mappings");
            
      std::map<int, int>::const_iterator itr = _notePositions.begin();
      while (itr != _notePositions.end()) {
            int tpc = itr->first;
            QString name = tpc2name(tpc, NoteSpellingType::STANDARD, false, false);
            
            xml.stag(QString("note name=\"%1\" tpc=\"%2\"").arg(name).arg(tpc));
            xml.tag("line-offset", itr->second);
            
            QString groupName;
            NoteHead::Group group = _noteHeads.at(tpc);
            if (group == NoteHead::Group::HEAD_SLASH)
                  groupName = "slash";
            else if (group == NoteHead::Group::HEAD_TRIANGLE)
                  groupName = "triangle";
            else if (group == NoteHead::Group::HEAD_DIAMOND)
                  groupName = "diamond";
            else if (group == NoteHead::Group::HEAD_CROSS)
                  groupName = "x";
            else if (group == NoteHead::Group::HEAD_XCIRCLE)
                  groupName = "circle-x";
            else if (group == NoteHead::Group::HEAD_DO)
                  groupName = "do";
            else if (group == NoteHead::Group::HEAD_RE)
                  groupName = "re";
            else if (group == NoteHead::Group::HEAD_MI)
                  groupName = "mi";
            else if (group == NoteHead::Group::HEAD_FA)
                  groupName = "fa";
            else if (group == NoteHead::Group::HEAD_LA)
                  groupName = "la";
            else if (group == NoteHead::Group::HEAD_TI)
                  groupName = "ti";
            else if (group == NoteHead::Group::HEAD_SOL)
                  groupName = "so";
            
            xml.tag("notehead-group", groupName);
            xml.etag();
            }
            
      xml.etag();
      }

void NoteMappings::readMappings(XmlReader& e)
      {
      while (e.readNextStartElement() && (e.name().toString() == "note")) {
            int tpc = e.attribute("tpc", "-2").toInt();
            
            while (e.readNextStartElement()) {
                  QString tag = e.name().toString();
                  if (tag == "line-offset") {
                        int offset = e.readInt();
                        _notePositions[tpc] = offset;
                        }
                  else if (tag == "notehead-group") {
                        QString text = e.readElementText();
                  
                        NoteHead::Group group;
                        if (text == "normal")
                              group = NoteHead::Group::HEAD_NORMAL;
                        else if (text == "cross")
                              group = NoteHead::Group::HEAD_CROSS;
                        else if (text == "diamond")
                              group = NoteHead::Group::HEAD_DIAMOND;
                        else if (text == "triangle")
                              group = NoteHead::Group::HEAD_TRIANGLE;
                        else if (text == "mi")
                              group = NoteHead::Group::HEAD_MI;
                        else if (text == "slash")
                              group = NoteHead::Group::HEAD_SLASH;
                        else if (text == "xcircle")
                              group = NoteHead::Group::HEAD_XCIRCLE;
                        else if (text == "do")
                              group = NoteHead::Group::HEAD_DO;
                        else if (text == "re")
                              group = NoteHead::Group::HEAD_RE;
                        else if (text == "fa")
                              group = NoteHead::Group::HEAD_FA;
                        else if (text == "la")
                              group = NoteHead::Group::HEAD_LA;
                        else if (text == "ti")
                              group = NoteHead::Group::HEAD_TI;
                        else if (text == "sol")
                              group = NoteHead::Group::HEAD_SOL;
                        else if (text == "brevis_alt")
                              group = NoteHead::Group::HEAD_BREVIS_ALT;
                        else if (text == "groups")
                              group =  NoteHead::Group::HEAD_GROUPS;
                        else
                              throw (QObject::tr("file includes an unrecognized notehead group."));

                        _noteHeads[tpc] = group;
                        }
                  }
            }
      }
      
void NoteMappings::writeClefOffsets(Xml& xml) const
      {
      xml.stag("clefOffset");
      
      std::map<ClefType, int>::const_iterator itr = _clefOffsets.begin();
      while (itr != _clefOffsets.end()) {
            
            QString name;
            ClefType ct = itr->first;
            if (ct == ClefType::G)
                  name = "G";
            else if (ct == ClefType::G1)
                  name = "G8va";
            else if (ct == ClefType::G2)
                  name = "G15ma";
            else if (ct == ClefType::G3)
                  name = "G8vb";
            else if (ct == ClefType::F)
                  name = "F";
            else if (ct == ClefType::F8)
                  name = "F8vb";
            else if (ct == ClefType::F15)
                  name = "F15mb";
            else if (ct == ClefType::F_B)
                  name = "F3";
            else if (ct == ClefType::F_C)
                  name = "F5";
            else if (ct == ClefType::C1)
                  name = "C1";
            else if (ct == ClefType::C2)
                  name = "C2";
            else if (ct == ClefType::C3)
                  name = "C3";
            else if (ct == ClefType::C4)
                  name = "C4";
            else if (ct == ClefType::C5)
                  name = "C5";
            else if (ct == ClefType::G4)
                  name = "G1";
            else if (ct == ClefType::F_8VA)
                  name = "F8va";
            else if (ct == ClefType::F_15MA)
                  name = "F15ma";
            else if (ct == ClefType::INVALID)
                  name = "INVALID";
            
            xml.tag(QString("clef name=\"%1\"").arg(name), itr->second);
            }
      
      xml.etag();
      }

void NoteMappings::readClefOffsets(XmlReader& e)
      {
      while (e.readNextStartElement() && (e.name().toString() == "clef")) {
            QString name = e.attribute("name", "");
            
            ClefType ct;
            if (name == "G")
                  ct = ClefType::G;
            else if (name == "G8va")
                  ct = ClefType::G1;
            else if (name == "G15ma")
                  ct = ClefType::G2;
            else if (name == "G8vb")
                  ct = ClefType::G3;
            else if (name == "F")
                  ct = ClefType::F;
            else if (name == "F8vb")
                  ct = ClefType::F8;
            else if (name == "F15mb")
                  ct = ClefType::F15;
            else if (name == "F3")
                  ct = ClefType::F_B;
            else if (name == "F5")
                  ct = ClefType::F_C;
            else if (name == "C1")
                  ct = ClefType::C1;
            else if (name == "C2")
                  ct = ClefType::C2;
            else if (name == "C3")
                  ct = ClefType::C3;
            else if (name == "C4")
                  ct = ClefType::C4;
            else if (name == "C5")
                  ct = ClefType::C5;
            else if (name == "G1")
                  ct = ClefType::G4;
            else if (name == "F8va")
                  ct = ClefType::F_8VA;
            else if (name == "F15ma")
                  ct = ClefType::F_15MA;
            else if (name == "INVALID")
                  ct = ClefType::INVALID;
            else
                  throw (QObject::tr("file includes an unrecognized clef type."));;

            _clefOffsets[ct] = e.readInt();
            }
      }

}