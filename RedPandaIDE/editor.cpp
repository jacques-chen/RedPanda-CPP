#include "editor.h"

#include <QtCore/QFileInfo>
#include <QFont>
#include <QTextCodec>
#include <QVariant>
#include <QWheelEvent>
#include <memory>
#include "settings.h"
#include "mainwindow.h"
#include <Qsci/qscilexercpp.h>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

using namespace std;


Editor::Editor(QWidget *parent, const QString& filename,
                  const QByteArray& encoding,
                  bool inProject, bool isNew,
                  QTabWidget* parentPageControl):
  QsciScintilla(parent),
  mFilename(filename),
  mEncodingOption(encoding),
  mInProject(inProject),
  mIsNew(isNew),
  mParentPageControl(parentPageControl)
{
    if (mFilename.isEmpty()) {
        mFilename = tr("untitled") + "1";
    }
    QFileInfo fileInfo(mFilename);
    if (mParentPageControl!=NULL) {
        mParentPageControl->addTab(this,QString());
        updateCaption();
    }
    if (!isNew) {
        loadFile();
    } else {
        if (mEncodingOption == ENCODING_AUTO_DETECT)
            mFileEncoding = ENCODING_ASCII;
        else
            mFileEncoding = mEncodingOption;
    }

    //
    QsciLexerCPP *lexer = new QsciLexerCPP();
    lexer->setHighlightEscapeSequences(true);
    lexer->setFoldComments(true);
    this->setLexer(lexer);
    this->setAutoIndent(pSettings->value(EDITOR_AUTO_INDENT).toBool());
    this->setFolding(FoldStyle::BoxedTreeFoldStyle,3);

    //行号显示区域
    setMarginType(0, QsciScintilla::NumberMargin);
    setMarginLineNumbers(0, true);
    setMarginWidth(0,30);
    //断点设置区域
    setMarginType(1, QsciScintilla::SymbolMargin);
    setMarginLineNumbers(1, false);
    setMarginWidth(1,20);
    setMarginSensitivity(1, true);    //设置是否可以显示断点
    setMarginsBackgroundColor(QColor("#bbfaae"));
    setMarginMarkerMask(1, 0x02);
//    connect(textEdit, SIGNAL(marginClicked(int, int, Qt::KeyboardModifiers)),this,
//            SLOT(on_margin_clicked(int, int, Qt::KeyboardModifiers)));
    markerDefine(QsciScintilla::Circle, 1);
    setMarkerBackgroundColor(QColor("#ee1111"), 1);
    //单步执行显示区域
    setMarginType(2, QsciScintilla::SymbolMargin);
    setMarginLineNumbers(2, false);
    setMarginWidth(2, 20);
    setMarginSensitivity(2, false);
    setMarginMarkerMask(2, 0x04);
    markerDefine(QsciScintilla::RightArrow, 2);
    setMarkerBackgroundColor(QColor("#eaf593"), 2);
    //自动折叠区域
    setMarginType(3, QsciScintilla::SymbolMargin);
    setMarginLineNumbers(3, false);
    setMarginWidth(3, 15);
    setMarginSensitivity(3, true);

    // connect will fail if use new function pointer syntax
//    connect(this, &QsciScintilla::modificationChanged,
//            this, &Editor::onModificationChanged);
//    connect(this , &QsciScintilla::cursorPositionChanged,
//            this, &Editor::onCursorPositionChanged);
    connect(this,SIGNAL(modificationChanged(bool)),
            this,SLOT(onModificationChanged(bool)));
    connect(this , SIGNAL(cursorPositionChanged(int,int)),
            this, SLOT(onCursorPositionChanged(int,int)));
    connect(this, SIGNAL(linesChanged()),
            this,SLOT(onLinesChanged()));
}

Editor::~Editor() {
    if (mParentPageControl!=NULL) {
        int index = mParentPageControl->indexOf(this);
        mParentPageControl->removeTab(index);
    }
    this->setParent(0);

    delete this->lexer();
    this->setLexer(NULL);
}

void Editor::loadFile() {
    QFile file(mFilename);
    QByteArray content=file.read(file.bytesAvailable());
    if (mEncodingOption == ENCODING_AUTO_DETECT) {
        mFileEncoding = GuessTextEncoding(content);
    } else {
        mFileEncoding = mEncodingOption;
    }
    if (mFileEncoding == ENCODING_UTF8) {
        this->setText(QString::fromUtf8(content));
    } else if (mFileEncoding == ENCODING_UTF8_BOM) {
        this->setText(QString::fromUtf8(content.mid(3)));
    } else if (mFileEncoding == ENCODING_ASCII) {
        this->setText(QString::fromLatin1(content));
    }else {
        QTextCodec*codec = QTextCodec::codecForName(mFileEncoding);
        this->setText(codec->toUnicode(content));
    }
}

void Editor::saveFile(const QString &filename) {
    if (mEncodingOption!=ENCODING_AUTO_DETECT && mEncodingOption!=mFileEncoding)  {
        mFileEncoding = mEncodingOption;
    }
    if (mEncodingOption == ENCODING_AUTO_DETECT && mFileEncoding == ENCODING_ASCII) {
        if (!isTextAllAscii(this->text())) {
            mFileEncoding = pSettings->value(EDITOR_DEFAULT_ENCODING).toByteArray();
        }
        pMainWindow->updateStatusBarForEncoding();
        //todo: update status bar, and set fileencoding using configurations
    }
    QFile file(filename);
    QByteArray ba;
    if (mFileEncoding == ENCODING_UTF8) {
        ba = this->text().toUtf8();
    } else if (mFileEncoding == ENCODING_UTF8_BOM) {
            ba.resize(3);
            ba[0]=0xEF;
            ba[1]=0xBB;
            ba[2]=0xBF;
            ba.append(this->text().toUtf8());
    } else if (mFileEncoding == ENCODING_ASCII) {
        ba = this->text().toLatin1();
    } else {
        QTextCodec* codec = QTextCodec::codecForName(mFileEncoding);
        ba = codec->fromUnicode(this->text());
    }
    file.open(QFile::WriteOnly);
    file.write(ba);
    file.close();
}

bool Editor::save(bool force, bool reparse) {
    if (this->mIsNew) {
        return saveAs();
    }
    QFile file(mFilename);
    QFileInfo info(mFilename);
    if (!force && !info.isWritable()) {
        QMessageBox::information(pMainWindow,tr("Fail"),
                                 QString(QObject::tr("File %s is not writable!")));
        return false;
    }
    //is this file read-only?
    if (this->isModified() || force) {
        saveFile(mFilename);
        setModified(false);
    }

    if (reparse) {
        //todo: reparse the file
    }
    return true;
}

bool Editor::saveAs(){
    QString newName = QFileDialog::getSaveFileName(pMainWindow,
                                                   tr("Save As"));
    if (newName.isEmpty()) {
        return false;
    }
    saveFile(newName);
    mFilename = newName;
    mIsNew = false;
    setModified(false);

    //todo: update (reassign highlighter)
    //todo: remove old file from parser and reparse file
    //todo: unmoniter/ monitor file
    //todo: update windows caption
    //todo: update class browser;
    return true;
}

const QByteArray& Editor::encodingOption() const {
    return mEncodingOption;
}
void Editor::setEncodingOption(const QByteArray& encoding) {
    mEncodingOption = encoding;
}
const QByteArray& Editor::fileEncoding() const {
    return mFileEncoding;
}
const QString& Editor::filename() {
    return mFilename;
}
bool Editor::inProject() const {
    return mInProject;
}
bool Editor::isNew() const {
    return mIsNew;
}

QTabWidget* Editor::pageControl() {
    return mParentPageControl;
}

void Editor::wheelEvent(QWheelEvent *event) {
    if ( (event->modifiers() & Qt::ControlModifier)!=0) {
        if (event->angleDelta().y()>0) {
            this->zoomIn();
        } else {
            this->zoomOut();
        }
    }
}

void Editor::onModificationChanged(bool status) {
    updateCaption();
}

void Editor::onCursorPositionChanged(int line, int index)
{
    pMainWindow->updateStatusBarForEditingInfo(line,index+1,lines(),text().length());
}

void Editor::onLinesChanged()
{
    qDebug()<<"lala"<<lines();
}

void Editor::updateCaption(const QString& newCaption) {
    if (mParentPageControl==NULL) {
        return;
    }
    int index = mParentPageControl->indexOf(this);
    if (index==-1)
        return;
    if (newCaption.isEmpty()) {
        QString caption = QFileInfo(mFilename).fileName();
        if (this->isModified()) {
            caption.append("[*]");
        }
        mParentPageControl->setTabText(index,caption);
    } else {
        mParentPageControl->setTabText(index,newCaption);
    }

}
