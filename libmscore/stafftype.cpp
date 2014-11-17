//=============================================================================
//  MuseScore
//  Music Composition & Notation
//
//  Copyright (C) 2010-2011 Werner Schweer
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License version 2
//  as published by the Free Software Foundation and appearing in
//  the file LICENCE.GPL
//=============================================================================

#include "stafftype.h"
#include "staff.h"
#include "xml.h"
#include "mscore.h"
#include "chord.h"

#define TAB_DEFAULT_LINE_SP   (1.5)
#define TAB_RESTSYMBDISPL     2.0

namespace Ms {

extern bool useFactorySettings;

//---------------------------------------------------------
//   StaffTypeTablature
//---------------------------------------------------------

#define TAB_DEFAULT_DUR_YOFFS (-1.0)

QList<TablatureFretFont>     StaffType::_fretFonts      = QList<TablatureFretFont>();
QList<TablatureDurationFont> StaffType::_durationFonts  = QList<TablatureDurationFont>();

const char StaffType::groupNames[STAFF_GROUP_MAX][STAFF_GROUP_NAME_MAX_LENGTH] = {
      QT_TRANSLATE_NOOP("Staff type group name", "Standard"),
      QT_TRANSLATE_NOOP("Staff type group name", "Percussion"),
      QT_TRANSLATE_NOOP("Staff type group name", "Tablature")
      };

const QString StaffType::fileGroupNames[STAFF_GROUP_MAX] = { "pitched", "percussion", "tablature" };

//---------------------------------------------------------
//   StaffType
//---------------------------------------------------------

StaffType::StaffType()
      {
      // set reasonable defaults for type-specific members */
      setDurationFontName(_durationFonts[0].displayName);
      setFretFontName(_fretFonts[0].displayName);
      }

StaffType::StaffType(StaffGroup sg, const QString& xml, const QString& name, int lines, qreal lineDist, bool genClef,
   bool showBarLines, bool stemless, bool genTimeSig, bool genKeySig, bool showLedgerLines) :
   _group(sg), _xmlName(xml), _name(name), _lineDistance(Spatium(lineDist)), _genClef(genClef),
   _showBarlines(showBarLines), _slashStyle(stemless), _genTimesig(genTimeSig),
   _genKeysig(genKeySig), _showLedgerLines(showLedgerLines)
      {
      setLines(lines);
      }

StaffType::StaffType(StaffGroup sg, const QString& xml, const QString& name, int lines, qreal lineDist, bool genClef,
   bool showBarLines, bool stemless, bool genTimesig,
   const QString& durFontName, qreal durFontSize, qreal durFontUserY, qreal genDur,
   const QString& fretFontName, qreal fretFontSize, qreal fretFontUserY,
   bool linesThrough, TablatureMinimStyle minimStyle, bool onLines, bool showRests,
   bool stemsDown, bool stemThrough, bool upsideDown, bool useNumbers)
      {
      _group   = sg;
      _xmlName = xml;
      _name    = name;
      setLines(lines);
      setLineDistance(Spatium(lineDist));
      setGenClef(genClef);
      setShowBarlines(showBarLines);
      setSlashStyle(stemless);
      setGenTimesig(genTimesig);
      setDurationFontName(durFontName);
      setDurationFontSize(durFontSize);
      setDurationFontUserY(durFontUserY);
      setGenDurations(genDur);
      setFretFontName(fretFontName);
      setFretFontSize(fretFontSize);
      setFretFontUserY(fretFontUserY);
      setLinesThrough(linesThrough);
      setMinimStyle(minimStyle);
      setOnLines(onLines);
      setShowRests(showRests);
      setStemsDown(stemsDown);
      setStemsThrough(stemThrough);
      setUpsideDown(upsideDown);
      setUseNumbers(useNumbers);
      }

//-----------------------------------------------------//cc
//   StaffType (copy constructor)
//---------------------------------------------------------

StaffType::StaffType(const StaffType& source) : 
      _group(source._group),
      _xmlName(source._xmlName),
      _name(source._name),
      _lines(source._lines),
      _stepOffset(source._stepOffset),
      _lineDistance(source._lineDistance),
      _genClef(source._genClef),
      _showBarlines(source._showBarlines),
      _slashStyle(source._slashStyle),
      _genTimesig(source._genTimesig),
      _genKeysig(source._genKeysig),
      _showLedgerLines(source._showLedgerLines),
      _durationFontSize(source._durationFontSize),
      _durationFontUserY(source._durationFontUserY),
      _fretFontSize(source._fretFontSize),
      _fretFontUserY(source._fretFontUserY),
      _genDurations(source._genDurations),
      _linesThrough(source._linesThrough),
      _minimStyle(source._minimStyle),
      _onLines(source._onLines),
      _showRests(source._showRests),
      _stemsDown(source._stemsDown),
      _stemsThrough(source._stemsThrough),
      _upsideDown(source._upsideDown),
      _useNumbers(source._useNumbers),
      _durationBoxH(source._durationBoxH),
      _durationBoxY(source._durationBoxY),
      _durationFont(source._durationFont),
      _durationFontIdx(source._durationFontIdx),
      _durationYOffset(source._durationYOffset),
      _durationMetricsValid(source._durationMetricsValid),
      _fretBoxH(source._fretBoxH),
      _fretBoxY(source._fretBoxY),
      _fretFont(source._fretFont),
      _fretFontIdx(source._fretFontIdx),
      _fretYOffset(source._fretYOffset),
      _fretMetricsValid(source._fretMetricsValid),
      _refDPI(source._refDPI),
      _useAlternateNoteMappings(source._useAlternateNoteMappings),
      _useInnerLedgers(source._useInnerLedgers),
      _useAlternateStaffLines(source._useAlternateStaffLines),
      _alternativeStaffLines(source._alternativeStaffLines)
      {
      if (source._altNoteMappings) //if not NULL
            _altNoteMappings = new NoteMappings(*(source._altNoteMappings));
      else
            _altNoteMappings = source._altNoteMappings;
      
      std::map<qreal, std::vector<qreal>>::const_iterator itr = source._innerLedgers.begin();
      while(itr != source._innerLedgers.end()) {
            std::vector<qreal> innerVector = itr->second;
            _innerLedgers[itr->first] = innerVector;
            itr++;
            }
      }

//-----------------------------------------------------//cc
//   swap
//---------------------------------------------------------
      
void swap(StaffType& first, StaffType& second)
      {
      using std::swap;
      swap(first._group, second._group);
      swap(first._xmlName, second._xmlName);
      swap(first._name, second._name);
      swap(first._lines, second._lines);
      swap(first._stepOffset, second._stepOffset);
      swap(first._lineDistance, second._lineDistance);
      swap(first._genClef, second._genClef);
      swap(first._showBarlines, second._showBarlines);
      swap(first._slashStyle, second._slashStyle);
      swap(first._genTimesig, second._genTimesig);
      swap(first._genKeysig, second._genKeysig);
      swap(first._showLedgerLines, second._showLedgerLines);
      swap(first._durationFontSize, second._durationFontSize);
      swap(first._durationFontUserY, second._durationFontUserY);
      swap(first._fretFontSize, second._fretFontSize);
      swap(first._fretFontUserY, second._fretFontUserY);
      swap(first._genDurations, second._genDurations);
      swap(first._linesThrough, second._linesThrough);
      swap(first._minimStyle, second._minimStyle);
      swap(first._onLines, second._onLines);
      swap(first._showRests, second._showRests);
      swap(first._stemsDown, second._stemsDown);
      swap(first._stemsThrough, second._stemsThrough);
      swap(first._upsideDown, second._upsideDown);
      swap(first._useNumbers, second._useNumbers);
      swap(first._durationBoxH, second._durationBoxH);
      swap(first._durationBoxY, second._durationBoxY);
      swap(first._durationFont, second._durationFont);
      swap(first._durationFontIdx, second._durationFontIdx);
      swap(first._durationYOffset, second._durationYOffset);
      swap(first._durationMetricsValid, second._durationMetricsValid);
      swap(first._fretBoxH, second._fretBoxH);
      swap(first._fretBoxY, second._fretBoxY);
      swap(first._fretFont, second._fretFont);
      swap(first._fretFontIdx, second._fretFontIdx);
      swap(first._fretYOffset, second._fretYOffset);
      swap(first._fretMetricsValid, second._fretMetricsValid);
      swap(first._refDPI, second._refDPI);
      swap(first._useInnerLedgers, second._useInnerLedgers);
      swap(first._useAlternateNoteMappings, second._useAlternateNoteMappings);
      swap(first._useAlternateStaffLines, second._useAlternateStaffLines);
      swap(first._altNoteMappings, second._altNoteMappings);
      swap(first._innerLedgers, second._innerLedgers);
      swap(first._alternativeStaffLines, second._alternativeStaffLines);
      }
      
//-----------------------------------------------------//cc
//   operator=
//---------------------------------------------------------
      
StaffType& StaffType::operator=(StaffType other)
      {
      swap(*this, other);
      return *this;
      }
      
//-----------------------------------------------------//cc
//   StaffType (move constructor)
//---------------------------------------------------------

StaffType::StaffType(StaffType&& other) : StaffType()
      {
      swap(*this, other);
      }

//-----------------------------------------------------//cc
//   ~StaffType
//---------------------------------------------------------

StaffType::~StaffType() {
      delete _altNoteMappings;
      }

//---------------------------------------------------------
//   groupName
//---------------------------------------------------------

const char* StaffType::groupName() const
      {
      return groupName(_group);
      }

const char* StaffType::groupName(StaffGroup r)
      {
      if (r < StaffGroup::STANDARD || (int)r >= STAFF_GROUP_MAX)
            r = StaffGroup::STANDARD;
      return groupNames[(int)r];
      }

//---------------------------------------------------------
//   operator==
//---------------------------------------------------------

bool StaffType::operator==(const StaffType& st) const
      {
      if (!isSameStructure(st) || st._xmlName != _xmlName) {        // common to all type groups
            return false;
            }
      if (_group == StaffGroup::TAB) {                      // TAB-specific
            bool v = st._durationFontIdx  == _durationFontIdx
               && st._durationFontSize  == _durationFontSize
               && st._durationFontUserY == _durationFontUserY
               && st._fretFontIdx       == _fretFontIdx
               && st._fretFontSize      == _fretFontSize
               && st._fretFontUserY     == _fretFontUserY
               ;
            return v;
            }
      return true;
      }
      
//-----------------------------------------------------//cc
//   operator!=
//---------------------------------------------------------

bool StaffType::operator!=(const StaffType& st) const
      {
      return !(*this == st);
      }

//---------------------------------------------------------
//   isSameStructure
//
//    same as operator==, but ignores names and fonts
//---------------------------------------------------------

//cc TODO: possibly append new members here

bool StaffType::isSameStructure(const StaffType& st) const
      {
      if (st.group()         != group()                     // common to all type groups
         || st._lines        != _lines
         || st._stepOffset   != _stepOffset
         || st._lineDistance != _lineDistance
         || st._genClef      != _genClef
         || st._showBarlines != _showBarlines
         || st._slashStyle   != _slashStyle
         || st._genTimesig   != _genTimesig)
            return false;

      //cc
      if (_group == StaffGroup::TAB) {                      // common to pitched and percussion
            return st._genKeysig      == _genKeysig
               && st._showLedgerLines == _showLedgerLines
               ;
            }
      else if (_group == StaffGroup::STANDARD) {
            if (st._useAlternateNoteMappings == _useAlternateNoteMappings
                      && st._useInnerLedgers == _useInnerLedgers
                      && st._useAlternateStaffLines == _useAlternateStaffLines) {
                  
                  bool sameMappings = true;
                  bool sameLedgers = true;
                  bool sameStaffLines = true;
                  if (_useAlternateNoteMappings)
                        sameMappings = *(st._altNoteMappings) == *_altNoteMappings;
                  if (_useInnerLedgers)
                        sameLedgers = st._innerLedgers == _innerLedgers;
                  if (_useAlternateStaffLines)
                        sameStaffLines = st._useAlternateStaffLines == _useAlternateStaffLines;
                  
                  return sameMappings && sameLedgers && sameStaffLines
                         && st._genKeysig      == _genKeysig
                         && st._showLedgerLines == _showLedgerLines;
                  }
            else {
                  return false;
                  }
            }
      else {                                                // TAB-specific
            return st._genDurations == _genDurations
               && st._linesThrough  == _linesThrough
               && st._minimStyle    == _minimStyle
               && st._onLines       == _onLines
               && st._showRests     == _showRests
               && st._stemsDown     == _stemsDown
               && st._stemsThrough  == _stemsThrough
               && st._upsideDown    == _upsideDown
               && st._useNumbers    == _useNumbers
               ;
            }
      }

//-----------------------------------------------------//cc
//   setAlternativeStaffLines
//---------------------------------------------------------
      
void StaffType::setAlternativeStaffLines(std::vector<qreal>& positions, int staffHeight) {
      _alternativeStaffLines = positions;
      _lines = staffHeight;
      }

//---------------------------------------------------------
//   setLines
//---------------------------------------------------------

void StaffType::setLines(int val)
      {
      _lines = val;
      
      if (_group != StaffGroup::TAB) {
            switch(_lines) {
                  case 1:
                        _stepOffset = 0;
                        break;
                  case 2:
                        _stepOffset = -2;
                        break;
                  case 3:
                  default:
                        _stepOffset = 0;
                        break;
                  }
            }
      else
            _stepOffset = (val / 2 - 2) * 2;    // tab staff
      }

//---------------------------------------------------------
//   write
//---------------------------------------------------------

void StaffType::write(Xml& xml) const
      {
      xml.stag(QString("StaffType group=\"%1\"").arg(fileGroupNames[(int)_group]));
      if (!_xmlName.isEmpty())
            xml.tag("name", _xmlName);
      if (_lines != 5)
            xml.tag("lines", _lines);
      if (_lineDistance.val() != 1.0)
            xml.tag("lineDistance", _lineDistance.val());
      if (!_genClef)
            xml.tag("clef", _genClef);
      if (_slashStyle)
            xml.tag("slashStyle", _slashStyle);
      if (!_showBarlines)
            xml.tag("barlines", _showBarlines);
      if (!_genTimesig)
            xml.tag("timesig", _genTimesig);
      if (_group == StaffGroup::STANDARD || _group == StaffGroup::PERCUSSION) {
            if (!_genKeysig)
                  xml.tag("keysig", _genKeysig);
            if (!_showLedgerLines)
                  xml.tag("ledgerlines", _showLedgerLines);
            }
            if (_useInnerLedgers) {
                  writeInnerLedgers(xml);
                  }
            if (_altNoteMappings) {
                  _altNoteMappings->write(xml);
                  }
            if (_useAlternateStaffLines) {
                  writeStaffLines(xml);
                  }
      else {
            xml.tag("durations",        _genDurations);
            xml.tag("durationFontName", _durationFonts[_durationFontIdx].displayName);
            xml.tag("durationFontSize", _durationFontSize);
            xml.tag("durationFontY",    _durationFontUserY);
            xml.tag("fretFontName",     _fretFonts[_fretFontIdx].displayName);
            xml.tag("fretFontSize",     _fretFontSize);
            xml.tag("fretFontY",        _fretFontUserY);
            xml.tag("linesThrough",     _linesThrough);
            xml.tag("minimStyle",       int(_minimStyle));
            xml.tag("onLines",          _onLines);
            xml.tag("showRests",        _showRests);
            xml.tag("stemsDown",        _stemsDown);
            xml.tag("stemsThrough",     _stemsThrough);
            xml.tag("upsideDown",       _upsideDown);
            xml.tag("useNumbers",       _useNumbers);
            }
      xml.etag();
      }

//---------------------------------------------------------
//   read
//---------------------------------------------------------

void StaffType::read(XmlReader& e)
      {
      QString group = e.attribute("group", fileGroupNames[(int)StaffGroup::STANDARD]);
      if (group == fileGroupNames[(int)StaffGroup::TAB])
            _group = StaffGroup::TAB;
      else if (group == fileGroupNames[(int)StaffGroup::PERCUSSION])
            _group = StaffGroup::PERCUSSION;
      else if (group == fileGroupNames[(int)StaffGroup::STANDARD])
            _group = StaffGroup::STANDARD;
      else {
            qDebug("StaffType::read: unknown group: %s", qPrintable(group));
            _group = StaffGroup::STANDARD;
            }

      while (e.readNextStartElement()) {
            const QStringRef& tag(e.name());
            if (tag == "name")
                  setXmlName(e.readElementText());
            else if (tag == "lines")
                  setLines(e.readInt());
            else if (tag == "lineDistance")
                  setLineDistance(Spatium(e.readDouble()));
            else if (tag == "clef")
                  setGenClef(e.readInt());
            else if (tag == "slashStyle")
                  setSlashStyle(e.readInt());
            else if (tag == "barlines")
                  setShowBarlines(e.readInt());
            else if (tag == "timesig")
                  setGenTimesig(e.readInt());
            else if (tag == "keysig")
                  _genKeysig = e.readInt();
            else if (tag == "ledgerlines")
                  _showLedgerLines = e.readInt();
            else if (tag == "durations")
                  setGenDurations(e.readBool());
            else if (tag == "durationFontName")
                  setDurationFontName(e.readElementText());
            else if (tag == "durationFontSize")
                  setDurationFontSize(e.readDouble());
            else if (tag == "durationFontY")
                  setDurationFontUserY(e.readDouble());
            else if (tag == "fretFontName")
                  setFretFontName(e.readElementText());
            else if (tag == "fretFontSize")
                  setFretFontSize(e.readDouble());
            else if (tag == "fretFontY")
                  setFretFontUserY(e.readDouble());
            else if (tag == "linesThrough")
                  setLinesThrough(e.readBool());
            else if (tag == "minimStyle")
                  setMinimStyle( (TablatureMinimStyle) e.readInt() );
            else if (tag == "onLines")
                  setOnLines(e.readBool());
            else if (tag == "showRests")
                  setShowRests(e.readBool());
            else if (tag == "stemsDown")
                  setStemsDown(e.readBool());
            else if (tag == "stemsThrough")
                  setStemsThrough(e.readBool());
            else if (tag == "upsideDown")
                  setUpsideDown(e.readBool());
            else if (tag == "useNumbers")
                  setUseNumbers(e.readBool());
            else if (tag == "innerLedgers")
                  readInnerLedgers(e);            //cc
            else if (tag == "staffLines")
                  readStaffLines(e);              //cc
            else if (tag == "noteMappings") {
                  _altNoteMappings = new NoteMappings();
                  _altNoteMappings->read(e); //cc
                  }
            else if (tag == "StaffType") //cc
                  continue;
            else
                  e.unknown();
            }
      }

//-----------------------------------------------------//cc
//   writeInnerLedgers
//---------------------------------------------------------

void StaffType::writeInnerLedgers(Xml& xml) const
      {
      xml.stag(QString("innerLedgers"));
      
      std::map<qreal, std::vector<qreal>>::const_iterator lineItr = _innerLedgers.begin();
      while (lineItr != _innerLedgers.end()) {
            QString lineVal = QString::number(lineItr->first,'f', 1);
            xml.stag(QString("note line=\"%1\"").arg(lineVal));
            
            std::vector<qreal>::const_iterator ledgerItr = lineItr->second.begin();
            while (ledgerItr != lineItr->second.end()) {
                  xml.tag("ledger", *ledgerItr);
                  ledgerItr++;
                  }
            
            xml.etag();
            lineItr++;
            }
            
      xml.etag();
      
      }

//-----------------------------------------------------//cc
//   readInnerLedgers
//---------------------------------------------------------

void StaffType::readInnerLedgers(XmlReader& e)
      {
      while (e.readNextStartElement()) {
            if (e.name().toString() == "note") {
                  qreal noteLine = e.intAttribute("line");
                  std::vector<qreal> ledgers = std::vector<qreal>(); //ledger lines associated with note
                  while (e.readNextStartElement()) {
                        ledgers.push_back(e.readInt());
                        }
                  if (!(ledgers.empty())) { //only add noteLine to map if ledgers exist for it
                        _innerLedgers[noteLine] = ledgers;
                        }
                  }
            }
      }

//-----------------------------------------------------//cc
//   writeStaffLines
//---------------------------------------------------------

void StaffType::writeStaffLines(Xml& xml) const
      {
      xml.stag(QString("staffLines"));
      std::vector<qreal>::const_iterator itr = _alternativeStaffLines.begin();
      while (itr != _alternativeStaffLines.end()) {
            xml.tag("line", *itr);
            itr++;
            }
      xml.etag();
      }

//-----------------------------------------------------//cc
//   readStaffLines
//---------------------------------------------------------

void StaffType::readStaffLines(XmlReader& e)
      {
      _alternativeStaffLines.clear();
      qreal cur = 0.0;
      qreal max = 0.0;
      while (e.readNextStartElement()) {
            if (e.name().toString() == "line") {
                  cur = e.readInt();
                  if (cur >= max)
                        max = cur;
                  _alternativeStaffLines.push_back(cur);
                  }
            }
      int staffHeight = (int) (max + 1.5); //round max up to the nearest int plus one
      setLines(staffHeight);
      }

//---------------------------------------------------------
//   doty1
//    get y dot position of first repeat barline dot
//---------------------------------------------------------

qreal StaffType::doty1() const
      {
      switch(_lines) {
            case 1:
                  return -_lineDistance.val() * .5;
            case 2:
                  return -_lineDistance.val() * .5;
            case 3:
                  return _lineDistance.val() * .5;
            case 4:
                  return _lineDistance.val() * .5;
            case 5:
                  return _lineDistance.val() * 1.5;
            case 6:
                  return _lineDistance.val() * 1.5;
            default:
                  qDebug("StaffType::doty1(): lines %d unsupported", _lines);
                  break;
            }
      return 0.0;
      }

//---------------------------------------------------------
//   doty2
//    get y dot position of second repeat barline dot
//---------------------------------------------------------

qreal StaffType::doty2() const
      {
      switch(_lines) {
            case 1:
                  return _lineDistance.val() * .5;
            case 2:
                  return _lineDistance.val() * 1.5;
            case 3:
                  return _lineDistance.val() * 1.5;
            case 4:
                  return _lineDistance.val() * 2.5;
            case 5:
                  return _lineDistance.val() * 2.5;
            case 6:
                  return _lineDistance.val() * 3.5;
            default:
                  qDebug("StaffType::doty2(): lines %d unsupported", _lines);
                  break;
            }
      return 0.0;
      }

//---------------------------------------------------------
//   setOnLines
//---------------------------------------------------------

void StaffType::setOnLines(bool val)
      {
      _onLines = val;
      _durationMetricsValid = _fretMetricsValid = false;
      }

//---------------------------------------------------------
//   set metrics
//    checks whether the internally computed metrics are is still valid and re-computes them, if not
//---------------------------------------------------------

void StaffType::setDurationMetrics()
      {
      if (_durationMetricsValid && _refDPI == MScore::DPI)           // metrics are still valid
            return;

      QFontMetricsF fm(durationFont());
      QString txt(_durationFonts[_durationFontIdx].displayValue, int(TabVal::NUM_OF));
      QRectF bb( fm.tightBoundingRect(txt) );
      // move symbols so that the lowest margin 'sits' on the base line:
      // move down by the whole part above (negative) the base line
      // ( -bb.y() ) then up by the whole height ( -bb.height()/2 )
      _durationYOffset = -bb.y() - bb.height()
      // then move up by a default margin and, if marks are above lines, by half the line distance
      // (converted from spatium units to raster units)
            + ( TAB_DEFAULT_DUR_YOFFS - (_onLines ? 0.0 : lineDistance().val()/2.0) ) * MScore::DPI*SPATIUM20;
      _durationBoxH = bb.height();
      _durationBoxY = bb.y()  + _durationYOffset;
      // keep track of the conditions under which metrics have been computed
      _refDPI = MScore::DPI;
      _durationMetricsValid = true;
      }

void StaffType::setFretMetrics()
      {
      if (_fretMetricsValid && _refDPI == MScore::DPI)
            return;

      QFontMetricsF fm(fretFont());
      QRectF bb;
      // compute vertical displacement
      if (_useNumbers) {
            // compute total height of used characters
            QString txt = QString();
            for (int idx = 0; idx < 10; idx++)  // use only first 10 digits
                  txt.append(_fretFonts[_fretFontIdx].displayDigit[idx]);
            bb = fm.tightBoundingRect(txt);
            // for numbers: centre on '0': move down by the whole part above (negative)
            // the base line ( -bb.y() ) then up by half the whole height ( -bb.height()/2 )
            QRectF bx( fm.tightBoundingRect(_fretFonts[_fretFontIdx].displayDigit[0]) );
            _fretYOffset = -(bx.y() + bx.height()/2.0);
            // _fretYOffset = -(bb.y() + bb.height()/2.0);  // <- using bbox of all chars
            }
      else {
            // compute total height of used characters
            QString txt(_fretFonts[_fretFontIdx].displayLetter, NUM_OF_LETTERFRETS);
            bb = fm.tightBoundingRect(txt);
            // for letters: centre on the 'a' ascender, by moving down half of the part above the base line in bx
            QRectF bx( fm.tightBoundingRect(_fretFonts[_fretFontIdx].displayLetter[0]) );
            _fretYOffset = -bx.y() / 2.0;
            }
      // if on string, we are done; if between strings, raise by half line distance
      if (!_onLines)
            _fretYOffset -= lineDistance().val()*MScore::DPI*SPATIUM20 / 2.0;

      // from _fretYOffset, compute _fretBoxH and _fretBoxY
      _fretBoxH = bb.height();
      _fretBoxY = bb.y()  + _fretYOffset;

      // keep track of the conditions under which metrics have been computed
      _refDPI = MScore::DPI;
      _fretMetricsValid = true;
      }

//---------------------------------------------------------
//   setDurationFontName / setFretFontName
//---------------------------------------------------------

void StaffType::setDurationFontName(const QString& name)
      {
      int idx;
      for (idx = 0; idx < _durationFonts.size(); idx++)
            if (_durationFonts[idx].displayName == name)
                  break;
      if (idx >= _durationFonts.size())
            idx = 0;          // if name not found, use first font
      _durationFont.setFamily(_durationFonts[idx].family);
      _durationFontIdx = idx;
      _durationMetricsValid = false;
      }

void StaffType::setFretFontName(const QString& name)
      {
      int idx;
      for (idx = 0; idx < _fretFonts.size(); idx++)
            if (_fretFonts[idx].displayName == name)
                  break;
      if (idx >= _fretFonts.size())
            idx = 0;          // if name not found, use first font
      _fretFont.setFamily(_fretFonts[idx].family);
      _fretFontIdx = idx;
      _fretMetricsValid = false;
      }

//---------------------------------------------------------
//   durationBoxH / durationBoxY
//---------------------------------------------------------

qreal StaffType::durationBoxH()
      {
      if (!_genDurations && !_slashStyle)
            return 0.0;
      setDurationMetrics();
      return _durationBoxH;
      }

qreal StaffType::durationBoxY()
      {
      if (!_genDurations && !_slashStyle)
            return 0.0;
      setDurationMetrics();
      return _durationBoxY + _durationFontUserY * MScore::MScore::DPI * SPATIUM20;
      }

//---------------------------------------------------------
//   setDurationFontSize / setFretFontSize
//---------------------------------------------------------

void StaffType::setDurationFontSize(qreal val)
      {
      _durationFontSize = val;
      _durationFont.setPixelSize( lrint(val * MScore::DPI / PPI) );
      _durationMetricsValid = false;
      }

void StaffType::setFretFontSize(qreal val)
      {
      _fretFontSize = val;
      _fretFont.setPixelSize( lrint(val * MScore::DPI / PPI) );
      _fretMetricsValid = false;
      }

//---------------------------------------------------------
//   chordRestStemPosY / chordStemPos / chordStemPosBeam / chordStemLength
//
//    computes the stem data for the given chord, according to TAB settings
//    NOTE: unit: spatium, position: relative to chord (DIFFERENT from Chord own functions)
//
//   chordRestStemPosY
//          returns the vertical position of stem start point
//---------------------------------------------------------

qreal StaffType::chordRestStemPosY(const ChordRest *chordRest) const
      {
      if (stemThrough())            // does not make sense for "stems through staves" setting; just return top line vert. position
            return 0.0;

      // if stems beside staff, position are fixed, but take into account delta for half notes
      qreal delta =                             // displacement for half note stems (if used)
            // if half notes have not a short stem OR not a half note => 0
            (minimStyle() != TablatureMinimStyle::SHORTER || chordRest->durationType().type() != TDuration::DurationType::V_HALF) ?
                  0.0 :
                  // if stem is up, displace of half stem length down (positive)
                  // if stem is down, displace of half stem length up (negative)
                  (chordRest->up() ?
                        -STAFFTYPE_TAB_DEFAULTSTEMLEN_UP : STAFFTYPE_TAB_DEFAULTSTEMLEN_DN) * 0.5;
      // if fret marks above lines and chordRest is up, move half a line distance up
      if (!onLines() && chordRest->up())
            delta -= _lineDistance.val() *0.5;
      qreal y = (chordRest->up() ? STAFFTYPE_TAB_DEFAULTSTEMPOSY_UP : (_lines-1)*_lineDistance.val() + STAFFTYPE_TAB_DEFAULTSTEMPOSY_DN)
            + delta;
      return y;
      }

//---------------------------------------------------------
//   chordStemPos
//    return position of note at other side of beam
//---------------------------------------------------------

QPointF StaffType::chordStemPos(const Chord* chord) const
      {
      qreal y;
      if (stemThrough())
            // if stems are through staff, stem goes from fartest note string
            y = (chord->up() ? chord->downString() : chord->upString()) * _lineDistance.val();
      else
            // if stems are beside staff, stem start point has a fixed vertical position,
            // according to TAB parameters and stem up/down
            y = chordRestStemPosY(chord);
      return QPointF(chordStemPosX(chord), y);
      }

//---------------------------------------------------------
//   chordStemPosBeam
//          return position of note at beam side of stem
//---------------------------------------------------------

QPointF StaffType::chordStemPosBeam(const Chord* chord) const
      {
      qreal y = ( stemsDown() ? chord->downString() : chord->upString() ) * _lineDistance.val();

      return QPointF(chordStemPosX(chord), y);
      }

//---------------------------------------------------------
//   chordStemLength
//          return length of stem
//---------------------------------------------------------

qreal StaffType::chordStemLength(const Chord* chord) const
      {
      qreal    stemLen;
      // if stems are through staff, length should be computed by relevant chord algorithm;
      // here, just return default length (= 1 'octave' = 3.5 line spaces)
      if (stemThrough())
            return STAFFTYPE_TAB_DEFAULTSTEMLEN_THRU * _lineDistance.val();
      // if stems beside staff, length is fixed, but take into account shorter half note stems
      else {
            bool shrt = (minimStyle() == TablatureMinimStyle::SHORTER) && (chord->durationType().type() == TDuration::DurationType::V_HALF);
            stemLen = (stemsDown() ? STAFFTYPE_TAB_DEFAULTSTEMLEN_DN : STAFFTYPE_TAB_DEFAULTSTEMLEN_UP)
                        * (shrt ? STAFFTYPE_TAB_SHORTSTEMRATIO : 1.0) * chord->mag();
            }
      return stemLen;
      }

//---------------------------------------------------------
//   fretString / durationString
//
//    construct the text string for a given fret / duration
//---------------------------------------------------------

static const QString unknownFret = QString("?");

QString StaffType::fretString(int fret, bool ghost) const
      {
      if (fret == FRET_NONE)
            return unknownFret;
      if (ghost)
            return _fretFonts[_fretFontIdx].ghostChar;
      else {
            if (_useNumbers) {
                  if(fret >= NUM_OF_DIGITFRETS)
                        return unknownFret;
                  else
                        return _fretFonts[_fretFontIdx].displayDigit[fret];
                  }
           else {
                  if(fret >= NUM_OF_LETTERFRETS)
                        return unknownFret;
                  else
                        return _fretFonts[_fretFontIdx].displayLetter[fret];
                  }
           }
      }

QString StaffType::durationString(TDuration::DurationType type, int dots) const
      {
      QString s = _durationFonts[_durationFontIdx].displayValue[int(type)];
      for(int count=0; count < dots; count++)
            s.append(_durationFonts[_durationFontIdx].displayDot);
      return s;
      }

//---------------------------------------------------------
//   physStringToVisual / VisualStringToPhys
//
//    returns the string ordinal in visual order (top to down) from a string ordinal in physical order
//    or viceversa: manage upsideDown
//
//    (The 2 functions are at the moment almost identical; support for unfrettted strings will
//    introduce more differences)
//---------------------------------------------------------

int StaffType::physStringToVisual(int strg) const
      {
      if(strg <= STRING_NONE || strg >= _lines)             // if no physical string, return topmost visual string
            return 0;
      // if TAB upside down, reverse string number
      return (_upsideDown ? _lines - 1 - strg : strg);
      }

int StaffType::visualStringToPhys(int strg) const
      {
      if(strg <= VISUAL_STRING_NONE || strg >= _lines)      // if no visual string, return topmost physical string
            return 0;
      // if TAB upside down, reverse string number
      return (_upsideDown ? _lines - 1 - strg : strg);
      }

//---------------------------------------------------------
//   TabDurationSymbol
//---------------------------------------------------------

TabDurationSymbol::TabDurationSymbol(Score* s)
   : Element(s)
      {
      setFlags(ElementFlag::MOVABLE | ElementFlag::SELECTABLE);
      setGenerated(true);
      _tab  = 0;
      _text = QString();
      }

TabDurationSymbol::TabDurationSymbol(Score* s, StaffType* tab, TDuration::DurationType type, int dots)
   : Element(s)
      {
      setFlags(ElementFlag::MOVABLE | ElementFlag::SELECTABLE);
      setGenerated(true);
      setDuration(type, dots, tab);
      }

TabDurationSymbol::TabDurationSymbol(const TabDurationSymbol& e)
   : Element(e)
      {
      _tab = e._tab;
      _text = e._text;
      }

//---------------------------------------------------------
//   layout
//---------------------------------------------------------

void TabDurationSymbol::layout()
      {
      if(!_tab) {
            setbbox(QRectF());
            return;
            }
      QFontMetricsF fm(_tab->durationFont());
      qreal mags = magS();
      qreal w = fm.width(_text);
      qreal y = _tab->durationBoxY();
      // with rests, move symbol down by half its displacement from staff
      if(parent() && parent()->type() == Element::Type::REST)
            y += TAB_RESTSYMBDISPL * spatium();
      bbox().setRect(0.0, y * mags, w * mags, _tab->durationBoxH() * mags);
      }

//---------------------------------------------------------
//   draw
//---------------------------------------------------------

void TabDurationSymbol::draw(QPainter* painter) const
      {
      if(!_tab)
            return;
      qreal mag = magS();
      qreal imag = 1.0 / mag;

      painter->setPen(curColor());
      painter->scale(mag, mag);
      painter->setFont(_tab->durationFont());
      qreal y = _tab->durationFontYOffset();
      if(parent() && parent()->type() == Element::Type::REST)
            y += TAB_RESTSYMBDISPL * spatium();
      painter->drawText(QPointF(0.0, y), _text);
      painter->scale(imag, imag);
      }

//---------------------------------------------------------
//   STATIC FUNCTIONS FOR FONT CONFIGURATION MANAGEMENT
//---------------------------------------------------------

bool TablatureFretFont::read(XmlReader& e)
      {
      defPitch = 9.0;
      defYOffset = 0.0;
      while (e.readNextStartElement()) {
            const QStringRef& tag(e.name());

            int val = e.intAttribute("value");

            if (tag == "family")
                  family = e.readElementText();
            else if (tag == "displayName")
                  displayName = e.readElementText();
            else if (tag == "defaultPitch")
                  defPitch = e.readDouble();
            else if (tag == "defaultYOffset")
                  defYOffset = e.readDouble();
            else if (tag == "mark") {
                  QString val = e.attribute("value");
                  QString txt(e.readElementText());
                  if (val.size() < 1)
                        return false;
                  if (val == "x")
                        xChar = txt[0];
                  else if (val == "ghost")
                        ghostChar = txt[0];
                  }
            else if (tag == "fret") {
                  bool bLetter = e.intAttribute("letter");
                  QString txt(e.readElementText());
                  if (bLetter) {
                        if (val >= 0 && val < NUM_OF_LETTERFRETS)
                              displayLetter[val] = txt[0];
                        }
                  else {
                        if (val >= 0 && val < NUM_OF_DIGITFRETS)
                              displayDigit[val] = txt;
                        }
                  }
            else {
                  e.unknown();
                  return false;
                  }
            }
      return true;
      }

bool TablatureDurationFont::read(XmlReader& e)
      {
      while (e.readNextStartElement()) {
            const QStringRef& tag(e.name());

            if (tag == "family")
                  family = e.readElementText();
            else if (tag == "displayName")
                  displayName = e.readElementText();
            else if (tag == "defaultPitch")
                  defPitch = e.readDouble();
            else if (tag == "defaultYOffset")
                  defYOffset = e.readDouble();
            else if (tag == "duration") {
                  QString val = e.attribute("value");
                  QString txt(e.readElementText());
                  QChar chr = txt[0];
                  if (val == "longa")
                        displayValue[int(TabVal::VAL_LONGA)] = chr;
                  else if (val == "brevis")
                        displayValue[int(TabVal::VAL_BREVIS)] = chr;
                  else if (val == "semibrevis")
                        displayValue[int(TabVal::VAL_SEMIBREVIS)] = chr;
                  else if (val == "minima")
                        displayValue[int(TabVal::VAL_MINIMA)] = chr;
                  else if (val == "semiminima")
                        displayValue[int(TabVal::VAL_SEMIMINIMA)] = chr;
                  else if (val == "fusa")
                        displayValue[int(TabVal::VAL_FUSA)] = chr;
                  else if (val == "semifusa")
                        displayValue[int(TabVal::VAL_SEMIFUSA)] = chr;
                  else if (val == "32")
                        displayValue[int(TabVal::VAL_32)] = chr;
                  else if (val == "64")
                        displayValue[int(TabVal::VAL_64)] = chr;
                  else if (val == "128")
                        displayValue[int(TabVal::VAL_128)] = chr;
                  else if (val == "256")
                        displayValue[int(TabVal::VAL_256)] = chr;
                  else if (val == "dot")
                        displayDot = chr;
                  else
                        e.unknown();
                  }
            else {
                  e.unknown();
                  return false;
                  }
            }
      return true;
      }

//---------------------------------------------------------
//   Read Configuration File
//
//    reads a configuration and appends read data to g_TABFonts
//    resets everythings and reads the built-in config file if fileName is null or empty
//---------------------------------------------------------

bool StaffType::readConfigFile(const QString& fileName)
      {
      QString path;

      if (fileName == 0 || fileName.isEmpty()) {       // defaults to built-in xml
#ifdef Q_OS_IOS
            {
            extern QString resourcePath();
            QString rpath = resourcePath();
            path = rpath + QString("/fonts_tablature.xml");
            }
#else
            path = ":/fonts/fonts_tablature.xml";
#endif
            _durationFonts.clear();
            _fretFonts.clear();
            }
      else
            path = fileName;

      QFileInfo fi(path);
      QFile f(path);

      if (!fi.exists() || !f.open(QIODevice::ReadOnly)) {
            MScore::lastError = QObject::tr("Cannot open tablature font description:\n%1\n%2").arg(f.fileName()).arg(f.errorString());
            qDebug("StaffTypeTablature::readConfigFile failed: <%s>", qPrintable(path));
            return false;
            }

      XmlReader e(&f);
      while (e.readNextStartElement()) {
            if (e.name() == "museScore") {
                  while (e.readNextStartElement()) {
                        const QStringRef& tag(e.name());
                        if (tag == "fretFont") {
                              TablatureFretFont f;
                              if (f.read(e))
                                    _fretFonts.append(f);
                              else
                                    continue;
                              }
                        else if (tag == "durationFont") {
                              TablatureDurationFont f;
                              if (f.read(e))
                                    _durationFonts.append(f);
                              else
                                    continue;
                              }
                        else
                              e.unknown();
                        }
                  return true;
                  }
            }
      return false;
      }

//---------------------------------------------------------
//   Get Font Names
//
//    returns a list of display names for the fonts  configured to work with Tablatures;
//    the index of a name in the list can be used to retrieve the font data with fontData()
//---------------------------------------------------------

QList<QString> StaffType::fontNames(bool bDuration)
      {
      QList<QString> names;
      if(bDuration)
            foreach(const TablatureDurationFont& f, _durationFonts)
                  names.append(f.displayName);
      else
            foreach(const TablatureFretFont& f, _fretFonts)
                  names.append(f.displayName);
      return names;
      }

//---------------------------------------------------------
//   Get Font Data
//
//    retrieves data about a Tablature font.
//    returns: true if idx is valid | false if it is not
// any of the pointer parameter can be null, if that datum is not needed
//---------------------------------------------------------

bool StaffType::fontData(bool bDuration, int nIdx, QString* pFamily, QString* pDisplayName,
   qreal* pSize, qreal* pYOff)
      {
      if (bDuration) {
            if (nIdx >= 0 && nIdx < _durationFonts.size()) {
                  TablatureDurationFont f = _durationFonts.at(nIdx);
                  if (pFamily)      *pFamily          = f.family;
                  if (pDisplayName) *pDisplayName     = f.displayName;
                  if (pSize)        *pSize            = f.defPitch;
                  if (pYOff)        *pYOff            = f.defYOffset;
                  return true;
                  }
            }
      else {
            if (nIdx >= 0 && nIdx < _fretFonts.size()) {
                  TablatureFretFont f = _fretFonts.at(nIdx);
                  if (pFamily)      *pFamily          = f.family;
                  if (pDisplayName) *pDisplayName     = f.displayName;
                  if (pSize)        *pSize            = f.defPitch;
                  if (pYOff)        *pYOff            = f.defYOffset;
                  return true;
                  }
            }
      return false;
      }

//=========================================================
//
//   BUILT-IN STAFF TYPES and STAFF TYPE PRESETS
//
//=========================================================

const int StaffType::_defaultPreset[] =
      { 0,              // default pitched preset is "stdNormal"
        3,              // default percussion preset is "perc5lines"
        5               // default tab preset is "tab6StrCommon"
      };

static const QString _emptyString = QString();

//---------------------------------------------------------
//   Static functions for StaffType presets
//---------------------------------------------------------

const StaffType* StaffType::preset(int idx) //cc
      {
      if (idx < 0 || idx >= int(_presets.size()))
            return &_presets[0];
      return &_presets[idx];
      }

const StaffType* StaffType::presetFromXmlName(QString& xmlName)
      {
      for (int i = 0; i < int(_presets.size()); ++i) {
            if (_presets[i].xmlName() == xmlName)
                  return &_presets[i];
            }
      return 0;
      }
#if 0
const StaffType* StaffType::presetFromName(QString& name)
      {
      for (int i = 0; i < (int)_presets.size(); ++i) {
            if (_presets[i].name() == name)
                  return &_presets[i];
            }
      return 0;
      }
#endif
const StaffType* StaffType::getDefaultPreset(StaffGroup grp)
      {
      int _idx = _defaultPreset[int(grp)];
      return &_presets[_idx];
      }

//-----------------------------------------------------//cc
//   copyUserTemplatesToPresets
//---------------------------------------------------------

std::vector<StaffType> StaffType::_presets;
      
void StaffType::copyUserTemplatesToPresets(std::vector<StaffTypeTemplate>& userTemplates)
      {
      //clear out the old userTemplates, if any
      std::vector<StaffType>::iterator start = _presets.begin() + _prebuiltTemplates.size(); //exclude prebuilt (hardcoded) templates
      std::vector<StaffType>::iterator stop  = _presets.end();
      if (start != stop) //,prevents undefined erase behavior
            _presets.erase(start, stop);
      
      //upcast StaffTypeTemplates to StaffTypes and append to _presets
      for (int i = 0; i < int(userTemplates.size()); i++) {
            StaffType* st = static_cast<StaffType*>(&userTemplates[i]);
            _presets.push_back(*st);
            }
      }


//---------------------------------------------------------
//   initStaffTypes
//---------------------------------------------------------

std::vector<StaffType> StaffType::_prebuiltTemplates;

void StaffType::initStaffTypes()
      {
      readConfigFile(0);          // get TAB font config, before initStaffTypes().

      _prebuiltTemplates = {
//                       group,              xml-name,  human-readable-name,        lin dst clef  bars stmless time  key  ledger
         StaffType(StaffGroup::STANDARD,   "stdNormal", QObject::tr("Standard"),      5, 1, true, true, false, true, true,  true),
         StaffType(StaffGroup::PERCUSSION, "perc1Line", QObject::tr("Perc. 1 line"),  1, 1, true, true, false, true, false, true),
         StaffType(StaffGroup::PERCUSSION, "perc3Line", QObject::tr("Perc. 3 lines"), 3, 2, true, true, false, true, false, true),
         StaffType(StaffGroup::PERCUSSION, "perc5Line", QObject::tr("Perc. 5 lines"), 5, 1, true, true, false, true, false, true),
//                 group               xml-name,         human-readable-name         lin dist  clef   bars stemless time      duration font     size off genDur     fret font         size off  thru  minim style       onLin  rests  stmDn  stmThr upsDn  nums
         StaffType(StaffGroup::TAB, "tab6StrSimple", QObject::tr("Tab. 6-str simple"), 6, 1.5, true,  true, true,  false, "MuseScore Tab Modern", 15, 0, false, "MuseScore Tab Sans",    9, 0, false, TablatureMinimStyle::NONE,   true,  false, true,  false, false, true),
         StaffType(StaffGroup::TAB, "tab6StrCommon", QObject::tr("Tab. 6-str common"), 6, 1.5, true,  true, false, false, "MuseScore Tab Modern", 15, 0, false, "MuseScore Tab Serif",   9, 0, false, TablatureMinimStyle::SHORTER,true,  false, true,  false, false, true),
         StaffType(StaffGroup::TAB, "tab6StrFull",   QObject::tr("Tab. 6-str full"),   6, 1.5, true,  true, false, true,  "MuseScore Tab Modern", 15, 0, false, "MuseScore Tab Serif",   9, 0, false, TablatureMinimStyle::SLASHED,true,  true,  true,  true,  false, true),
         StaffType(StaffGroup::TAB, "tab4StrSimple", QObject::tr("Tab. 4-str simple"), 4, 1.5, true,  true, true,  false, "MuseScore Tab Modern", 15, 0, false, "MuseScore Tab Sans",    9, 0, false, TablatureMinimStyle::NONE,   true,  false, true,  false, false, true),
         StaffType(StaffGroup::TAB, "tab4StrCommon", QObject::tr("Tab. 4-str common"), 4, 1.5, true,  true, false, false, "MuseScore Tab Modern", 15, 0, false, "MuseScore Tab Serif",   9, 0, false, TablatureMinimStyle::SHORTER,true,  false, true,  false, false, true),
         StaffType(StaffGroup::TAB, "tab4StrFull",   QObject::tr("Tab. 4-str full"),   4, 1.5, true,  true, false, false, "MuseScore Tab Modern", 15, 0, false, "MuseScore Tab Serif",   9, 0, false, TablatureMinimStyle::SLASHED,true,  true,  true,  true,  false, true),
         StaffType(StaffGroup::TAB, "tab5StrSimple", QObject::tr("Tab. 5-str simple"), 5, 1.5, true,  true, true,  false, "MuseScore Tab Modern", 15, 0, false, "MuseScore Tab Sans",    9, 0, false, TablatureMinimStyle::NONE,   true,  false, true,  false, false, true),
         StaffType(StaffGroup::TAB, "tab5StrCommon", QObject::tr("Tab. 5-str common"), 5, 1.5, true,  true, false, false, "MuseScore Tab Modern", 15, 0, false, "MuseScore Tab Serif",   9, 0, false, TablatureMinimStyle::SHORTER,true,  false, true,  false, false, true),
         StaffType(StaffGroup::TAB, "tab5StrFull",   QObject::tr("Tab. 5-str full"),   5, 1.5, true,  true, false, false, "MuseScore Tab Modern", 15, 0, false, "MuseScore Tab Serif",   9, 0, false, TablatureMinimStyle::SLASHED,true,  true,  true,  true,  false, true),
         StaffType(StaffGroup::TAB, "tabUkulele",    QObject::tr("Tab. ukulele"),      4, 1.5, true,  true, false, false, "MuseScore Tab Modern", 15, 0, false, "MuseScore Tab Serif",   9, 0, false, TablatureMinimStyle::SHORTER,true,  true,  true,  false, false, true),
         StaffType(StaffGroup::TAB, "tabBalajka",    QObject::tr("Tab. balalaika"),    3, 1.5, true,  true, false, false, "MuseScore Tab Modern", 15, 0, false, "MuseScore Tab Serif",   9, 0, false, TablatureMinimStyle::SHORTER,true,  true,  true,  false, false, true),
         StaffType(StaffGroup::TAB, "tab6StrItalian",QObject::tr("Tab. 6-str Italian"),6, 1.5, false, true, true,  true,  "MuseScore Tab Italian",15, 0, true,  "MuseScore Tab Renaiss",10, 0, true,  TablatureMinimStyle::NONE,   true,  true,  false, false, true,  true),
         StaffType(StaffGroup::TAB, "tab6StrFrench", QObject::tr("Tab. 6-str French"), 6, 1.5, false, true, true,  true,  "MuseScore Tab French", 15, 0, true,  "MuseScore Tab Renaiss",10, 0, true,  TablatureMinimStyle::NONE,   false, false, false, false, false, false)
         };
         
      //cc add prebuilt templates to presets
      for (int i = 0; i < int(_prebuiltTemplates.size()); i++) {
            _presets.emplace_back(_prebuiltTemplates[i]);
            }
      }

//-----------------------------------------------------//cc
//   StaffTypeTemplate
//---------------------------------------------------------

StaffTypeTemplate::StaffTypeTemplate() :
      StaffType(StaffGroup::STANDARD, "", QObject::tr(""), 5, 1, true, true, false, true, true, true)
      {
      _useInnerLedgers = true;
      _useAlternateNoteMappings = true;
      _useAlternateStaffLines = true;
      
      _altNoteMappings = new NoteMappings(); //TODO: when copying stafftypetemplate
      _alternativeStaffLines.push_back(0);   //      over to a stafftype, make sure
      _alternativeStaffLines.push_back(1);   //      to make a "deep" copy of
      _alternativeStaffLines.push_back(2);   //      _altNoteMappings
      _alternativeStaffLines.push_back(3);
      _alternativeStaffLines.push_back(4);
      }
      
//-----------------------------------------------------//cc
//   StaffTypeTemplate
//---------------------------------------------------------

void StaffTypeTemplate::setFileName(QString s)
      {
      if (s.isEmpty()) {
            QFileInfo f;
            _fileInfo = f;
            }
      else
            _fileInfo.setFile(s);
      }

//-----------------------------------------------------//cc
//   operator=
//---------------------------------------------------------
      
//      TODO: THIS SEEMS INCORRECT (overloading base class assignment operator)
      
StaffTypeTemplate& StaffTypeTemplate::operator=(StaffTypeTemplate other)
      {
      StaffType::operator=(other); //TODO: check what this actually means
      _dirty = other._dirty;
      _hasFile = other._hasFile;
      _fileInfo = other._fileInfo;
      
      return *this;
      }

//cc
std::vector<StaffTypeTemplate> StaffTypeTemplate::_userTemplates;
const int StaffTypeTemplate::STAFFTYPE_TEMPLATE_LIST_SIZE;

//-----------------------------------------------------//cc
//   initUserTemplates
//---------------------------------------------------------

void StaffTypeTemplate::initUserTemplates()
      {
      QSettings settings;
      for (int i = 0; i < STAFFTYPE_TEMPLATE_LIST_SIZE; i++) {
            QString path = settings.value(QString("user-stafftypes-%1").arg(i),"").toString();
            if (!path.isEmpty()) {
                  StaffTypeTemplate st;
                  try {
                        QFile f(path);
                        if (!f.open(QIODevice::ReadOnly))
                              throw QObject::tr("file failed to open");
                        XmlReader xml(&f);
                        st.read(xml);
                        st.setName(st.xmlName()); //name and xml name identical for user templates
                        st.setHasFile(true);
                        st.setDirty(false);
                        }
                  catch (QString error) {
                        settings.remove(QString("user-stafftypes-%1").arg(i));
                        QMessageBox::warning(0, QObject::tr("MuseScore: Load Error"),
                                                QObject::tr("Error loading StaffType Template \"%1\": %2\n"
                                                            "This template will no longer be included.").arg(path).arg(error));
                        continue;
                        }
                  st.setFileName(path);
                  _userTemplates.emplace_back(st); //TODO: Possibly convert _userTemplates into a list of
                                                 //      pointers instead (and then just push st).
                                                 //OR: create a copy constructor
                  }
            }
      StaffType::copyUserTemplatesToPresets(_userTemplates);
      }
      
//-----------------------------------------------------//cc
//   updateSettings
//---------------------------------------------------------
      
void StaffTypeTemplate::updateSettings() {
      QSettings settings;
      int i = 0;
      for (StaffTypeTemplate& st : _userTemplates) {
            settings.setValue(QString("user-stafftypes-%1").arg(i), st.fileInfo()->absoluteFilePath());
            i++;
            }
      while (i < STAFFTYPE_TEMPLATE_LIST_SIZE) {
            settings.remove(QString("user-stafftypes-%1").arg(i));
            i++;
            }

      }

//-----------------------------------------------------//cc
//   updateTemplate
//---------------------------------------------------------
      
void StaffTypeTemplate::addTemplate(StaffTypeTemplate& t)
      {
      _userTemplates.emplace_back(t);
      StaffType::copyUserTemplatesToPresets(_userTemplates);
      }
      
//-----------------------------------------------------//cc
//   updateTemplate
//---------------------------------------------------------

void StaffTypeTemplate::updateTemplate(StaffTypeTemplate& sttNew)
      {
      std::vector<StaffTypeTemplate>::iterator itr = _userTemplates.begin();
      while (itr != _userTemplates.end()) {
            if (itr->_fileInfo.absoluteFilePath() == sttNew._fileInfo.absoluteFilePath()) {
                  (*itr) = sttNew; //swap the old StaffType with the new one
                  break;
                  }
            itr++;
            }
      StaffType::copyUserTemplatesToPresets(_userTemplates);
      }

//-----------------------------------------------------//cc
//   removeTemplate
//---------------------------------------------------------
      
void StaffTypeTemplate::removeTemplate(StaffTypeTemplate& t)
      {
      std::vector<StaffTypeTemplate>::iterator itr = _userTemplates.begin();
      while (itr != _userTemplates.end()) {
            if (itr->_fileInfo.absoluteFilePath() == t._fileInfo.absoluteFilePath()) {
                  _userTemplates.erase(itr);
                  break;
                  }
            itr++;
            }
      StaffType::copyUserTemplatesToPresets(_userTemplates);
      }
      
}                 // namespace Ms

