/*****自制词法分析器头文件********/

#ifndef MYLEXERC_H
#define MYLEXERC_H

#include <QObject>

#include <Qsci/qsciglobal.h>
#include <Qsci/qscilexer.h>


class QSCINTILLA_EXPORT MyLexerC : public QsciLexer
{
    Q_OBJECT
public:
    //! This enum defines the meanings of the different styles used by the
    //! C++ lexer.
    enum {
        //! The default.
        Default = 0,
        InactiveDefault = Default + 64,

        //! A C comment.
        Comment = 1,
        InactiveComment = Comment + 64,

        //! A C++ comment line.
        CommentLine = 2,
        InactiveCommentLine = CommentLine + 64,

        //! A JavaDoc/Doxygen style C comment.
        CommentDoc = 3,
        InactiveCommentDoc = CommentDoc + 64,

        //! A number.
        Number = 4,
        InactiveNumber = Number + 64,

        //! A keyword.
        Keyword = 5,
        InactiveKeyword = Keyword + 64,

        //! A double-quoted string.
        DoubleQuotedString = 6,
        InactiveDoubleQuotedString = DoubleQuotedString + 64,

        //! A single-quoted string.
        SingleQuotedString = 7,
        InactiveSingleQuotedString = SingleQuotedString + 64,

        //! An IDL UUID.
        UUID = 8,
        InactiveUUID = UUID + 64,

        //! A pre-processor block.
        PreProcessor = 9,
        InactivePreProcessor = PreProcessor + 64,

        //! An operator.
        Operator = 10,
        InactiveOperator = Operator + 64,

        //! An identifier
        Identifier = 11,
        InactiveIdentifier = Identifier + 64,

        //! The end of a line where a string is not closed.
        UnclosedString = 12,
        InactiveUnclosedString = UnclosedString + 64,

        //! A C# verbatim string.
        VerbatimString = 13,
        InactiveVerbatimString = VerbatimString + 64,

        //! A JavaScript regular expression.
        Regex = 14,
        InactiveRegex = Regex + 64,

        //! A JavaDoc/Doxygen style C++ comment line.
        CommentLineDoc = 15,
        InactiveCommentLineDoc = CommentLineDoc + 64,

        //! A keyword defined in keyword set number 2.  The class must be
        //! sub-classed and re-implement keywords() to make use of this style.
        KeywordSet2 = 16,
        InactiveKeywordSet2 = KeywordSet2 + 64,

        //! A JavaDoc/Doxygen keyword.
        CommentDocKeyword = 17,
        InactiveCommentDocKeyword = CommentDocKeyword + 64,

        //! A JavaDoc/Doxygen keyword error.
        CommentDocKeywordError = 18,
        InactiveCommentDocKeywordError = CommentDocKeywordError + 64,

        //! A global class or typedef defined in keyword set number 5.  The
        //! class must be sub-classed and re-implement keywords() to make use
        //! of this style.
        GlobalClass = 19,
        InactiveGlobalClass = GlobalClass + 64,

        //! A C++ raw string.
        RawString = 20,
        InactiveRawString = RawString + 64,

        //! A Vala triple-quoted verbatim string.
        TripleQuotedVerbatimString = 21,
        InactiveTripleQuotedVerbatimString = TripleQuotedVerbatimString + 64,

        //! A Pike hash-quoted string.
        HashQuotedString = 22,
        InactiveHashQuotedString = HashQuotedString + 64,

        //! A pre-processor stream comment.
        PreProcessorComment = 23,
        InactivePreProcessorComment = PreProcessorComment + 64,

        //! A JavaDoc/Doxygen style pre-processor comment.
        PreProcessorCommentLineDoc = 24,
        InactivePreProcessorCommentLineDoc = PreProcessorCommentLineDoc + 64,

        //! A user-defined literal.
        UserLiteral = 25,
        InactiveUserLiteral = UserLiteral + 64,

        //! A task marker.
        TaskMarker = 26,
        InactiveTaskMarker = TaskMarker + 64,

        //! An escape sequence.
        EscapeSequence = 27,
        InactiveEscapeSequence = EscapeSequence + 64,
    };

    //! 创建一个MyLexerC类，设置父亲为一个QsciScintilla实例， caseInsensitiveKeywords
    //! 为真表示此词法分析器忽略关键字大小写
    MyLexerC(QObject *parent = 0, bool caseInsensitiveKeywords = false);    //父控件为一个QsciScintilla实例，

    //! MyLexerC析构函数
    virtual ~MyLexerC();

    //! Returns the name of the language.
    //! 返回语言的名字
    const char *language() const;

    //! Returns the name of the lexer.  Some lexers support a number of
    //! languages.
    //! 返回词法分析器语言的名字。有些词法分析器提供多种语言
    const char *lexer() const;

    //! \internal Returns the character sequences that can separate
    //! auto-completion words.
    //! 返回可以分割自动补全单词的字符串
    QStringList autoCompletionWordSeparators() const;


    //! \internal Returns a space separated list of words or characters in
    //! a particular style that define the end of a block for
    //! auto-indentation.  The styles is returned via \a style.
    //! 一个块的结尾
    //!
    const char *blockEnd(int *style = 0) const;

    //! \internal Returns a space separated list of words or characters in
    //! a particular style that define the start of a block for
    //! auto-indentation.  The styles is returned via \a style.
    //! 一个块的开头
    const char *blockStart(int *style = 0) const;

    //! \internal Returns a space separated list of keywords in a
    //! particular style that define the start of a block for
    //! auto-indentation.  The styles is returned via \a style.
    //! 也是一个块的开头
    const char *blockStartKeyword(int *style = 0) const;

    //! \internal Returns the style used for braces for brace matching.
    //!
    int braceStyle() const;

    //! Returns the string of characters that comprise a word.
    //! 返回包含了一个单词的字符串
    const char *wordCharacters() const;

    //! Returns the foreground colour of the text for style number \a style.
    //!
    //! \sa defaultPaper()
    //! 返回字体的前景颜色
    QColor defaultColor(int style) const;

    //! Returns the end-of-line fill for style number \a style.
    //! 返回。。。。
    bool defaultEolFill(int style) const;

    //! Returns the font for style number \a style.
    //! 返回字体
    QFont defaultFont(int style) const;

    //! Returns the background colour of the text for style number \a style.
    //!
    //! \sa defaultColor()
    //! 返回文本背景色
    QColor defaultPaper(int style) const;

    //! Returns the set of keywords for the keyword set \a set recognised
    //! by the lexer as a space separated string.  Set 1 is normally used for
    //! primary keywords and identifiers.  Set 2 is normally used for secondary
    //! keywords and identifiers.  Set 3 is normally used for documentation
    //! comment keywords.  Set 4 is normally used for global classes and
    //! typedefs.
    const char *keywords(int set) const;

    //! Returns the descriptive name for style number \a style.  If the
    //! style is invalid for this language then an empty QString is returned.
    //! This is intended to be used in user preference dialogs.
    //! 返回某一风格的名字
    QString description(int style) const;

    //! Causes all properties to be refreshed by emitting the
    //! propertyChanged() signal as required.
    void refreshProperties();

    //! Returns true if "} else {" lines can be folded.
    //!
    //! \sa setFoldAtElse()
    bool foldAtElse() const {return fold_atelse;}

    //! Returns true if multi-line comment blocks can be folded.
    //!
    //! \sa setFoldComments()
    //! 如果多行注释块可以被折叠，则返回真
    bool foldComments() const {return fold_comments;}

    //! Returns true if trailing blank lines are included in a fold block.
    //!
    //! \sa setFoldCompact()
    //! 如果尾部空行被包含在一个折叠的块中，则返回真
    bool foldCompact() const {return fold_compact;}

    //! Returns true if preprocessor blocks can be folded.
    //!
    //! \sa setFoldPreprocessor()
    //! 如果预处理块能被折叠，则返回真
    bool foldPreprocessor() const {return fold_preproc;}

    //! Returns true if preprocessor lines (after the preprocessor
    //! directive) are styled.
    //!
    //! \sa setStylePreprocessor()
    //! 如果预编译行具有风格则返回真
    bool stylePreprocessor() const {return style_preproc;}

    //! If \a allowed is true then '$' characters are allowed in identifier
    //! names.  The default is true.
    //!
    //! \sa dollarsAllowed()
    //! 是否允许$出现在变量命名中
    void setDollarsAllowed(bool allowed);

    //! Returns true if '$' characters are allowed in identifier names.
    //!
    //! \sa setDollarsAllowed()
    //! 是否允许$出现在变量命名中
    bool dollarsAllowed() const {return dollars;}

    //! If \a enabled is true then triple quoted strings are highlighted.  The
    //! default is false.
    //!
    //! \sa highlightTripleQuotedStrings()
    //!
    void setHighlightTripleQuotedStrings(bool enabled);

    //! Returns true if triple quoted strings should be highlighted.
    //!
    //! \sa setHighlightTripleQuotedStrings()
    bool highlightTripleQuotedStrings() const {return highlight_triple;}

    //! If \a enabled is true then hash quoted strings are highlighted.  The
    //! default is false.
    //!
    //! \sa highlightHashQuotedStrings()
    void setHighlightHashQuotedStrings(bool enabled);

    //! Returns true if hash quoted strings should be highlighted.
    //!
    //! \sa setHighlightHashQuotedStrings()
    bool highlightHashQuotedStrings() const {return highlight_hash;}

    //! If \a enabled is true then back-quoted raw strings are highlighted.
    //! The default is false.
    //!
    //! \sa highlightBackQuotedStrings()
    void setHighlightBackQuotedStrings(bool enabled);

    //! Returns true if back-quoted raw strings should be highlighted.
    //!
    //! \sa setHighlightBackQuotedStrings()
    bool highlightBackQuotedStrings() const {return highlight_back;}

    //! If \a enabled is true then escape sequences in strings are highlighted.
    //! The default is false.
    //!
    //! \sa highlightEscapeSequences()
    void setHighlightEscapeSequences(bool enabled);

    //! Returns true if escape sequences in strings should be highlighted.
    //!
    //! \sa setHighlightEscapeSequences()
    bool highlightEscapeSequences() const {return highlight_escape;}

    //! If \a allowed is true then escape sequences are allowed in verbatim
    //! strings.  The default is false.
    //!
    //! \sa verbatimStringEscapeSequencesAllowed()
    void setVerbatimStringEscapeSequencesAllowed(bool allowed);

    //! Returns true if hash quoted strings should be highlighted.
    //!
    //! \sa setVerbatimStringEscapeSequencesAllowed()
    bool verbatimStringEscapeSequencesAllowed() const {return vs_escape;}

public slots:
    //! If \a fold is true then "} else {" lines can be folded.  The
    //! default is false.
    //!
    //! \sa foldAtElse()
    virtual void setFoldAtElse(bool fold);

    //! If \a fold is true then multi-line comment blocks can be folded.
    //! The default is false.
    //!
    //! \sa foldComments()
    virtual void setFoldComments(bool fold);

    //! If \a fold is true then trailing blank lines are included in a fold
    //! block. The default is true.
    //!
    //! \sa foldCompact()
    virtual void setFoldCompact(bool fold);

    //! If \a fold is true then preprocessor blocks can be folded.  The
    //! default is true.
    //!
    //! \sa foldPreprocessor()
    virtual void setFoldPreprocessor(bool fold);

    //! If \a style is true then preprocessor lines (after the preprocessor
    //! directive) are styled.  The default is false.
    //!
    //! \sa stylePreprocessor()
    virtual void setStylePreprocessor(bool style);

protected:
    //! The lexer's properties are read from the settings \a qs.  \a prefix
    //! (which has a trailing '/') should be used as a prefix to the key of
    //! each setting.  true is returned if there is no error.
    //!
    //! \sa writeProperties()
    bool readProperties(QSettings &qs,const QString &prefix);

    //! The lexer's properties are written to the settings \a qs.
    //! \a prefix (which has a trailing '/') should be used as a prefix to
    //! the key of each setting.  true is returned if there is no error.
    //!
    //! \sa readProperties()
    bool writeProperties(QSettings &qs,const QString &prefix) const;

private:
    void setAtElseProp();
    void setCommentProp();
    void setCompactProp();
    void setPreprocProp();
    void setStylePreprocProp();
    void setDollarsProp();
    void setHighlightTripleProp();
    void setHighlightHashProp();
    void setHighlightBackProp();
    void setHighlightEscapeProp();
    void setVerbatimStringEscapeProp();

    bool fold_atelse;
    bool fold_comments;
    bool fold_compact;
    bool fold_preproc;
    bool style_preproc;
    bool dollars;
    bool highlight_triple;
    bool highlight_hash;
    bool highlight_back;
    bool highlight_escape;
    bool vs_escape;

    bool nocase;

    MyLexerC(const MyLexerC &);
    MyLexerC &operator=(const MyLexerC &);
};


#endif // MYLEXERC_H
