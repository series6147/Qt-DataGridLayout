#ifndef DATAGRID_H
#define DATAGRID_H

#include <QAbstractItemModel>
#include <QMutex>
#include <QQmlEngine>
#include <QQueue>
#include <QQuickItem>
#include <QQuickWindow>
#include <QSortFilterProxyModel>
#include <QTimer>

class DataGridColumn;
class DataGridHeaderPresenter;
class DataGridItemLayout;
class DataGridRowPresenter;
class DataGridSortFilterProxyModel;
class FilterAcceptsRowEventArgs;
class ObservableObjectEventArgs;
class DataGrid : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(bool backgroundEnabled READ backgroundEnabled WRITE setBackgroundEnabled NOTIFY backgroundEnabledChanged)
    Q_PROPERTY(bool isReadOnly READ isReadOnly WRITE setIsReadOnly NOTIFY isReadOnlyChanged)
    Q_PROPERTY(bool sortEnabled READ sortEnabled WRITE setSortEnabled NOTIFY sortEnabledChanged)
    Q_PROPERTY(DataGridItemLayout* itemLayout READ itemLayout)
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
    Q_PROPERTY(int itemHeight READ itemHeight WRITE setItemHeight NOTIFY itemHeightChanged)
    Q_PROPERTY(int rowCount READ rowCount NOTIFY rowCountChanged)
    Q_PROPERTY(QAbstractItemModel* model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(qreal headerHeight READ headerHeight NOTIFY headerHeightChanged)
    Q_PROPERTY(qreal layoutWidth READ layoutWidth NOTIFY layoutWidthChanged)
    Q_PROPERTY(QString alternativeRowBackground READ alternativeRowBackground WRITE setAlternativeRowBackground NOTIFY alternativeRowBackgroundChanged)
    Q_PROPERTY(QString headerBackground READ headerBackground WRITE setHeaderBackground NOTIFY headerBackgroundChanged)
    Q_PROPERTY(QString highlightColor READ highlightColor WRITE setHighlightColor NOTIFY highlightColorChanged)
    Q_PROPERTY(QString inactiveHighlightColor READ inactiveHighlightColor WRITE setInactiveHighlightColor NOTIFY inactiveHighlightColorChanged)
    Q_PROPERTY(QString observablePropertyName READ observablePropertyName WRITE setObservablePropertyName NOTIFY observablePropertyNameChanged)
    Q_PROPERTY(Qt::CaseSensitivity filterCaseSensitivity READ filterCaseSensitivity WRITE setFilterCaseSensitivity)
    Q_PROPERTY(SelectionMode selectionMode READ selectionMode WRITE setSelectionMode NOTIFY selectionModeChanged)
public:
    enum SelectionMode {
        NoSelection = 0,
        SingleRow = 1,
        MultipleRows = 2
    };

    Q_ENUM(SelectionMode)

    explicit DataGrid(QQuickItem *parent = Q_NULLPTR);
    bool backgroundEnabled() const;
    bool isReadOnly() const;
    bool sortEnabled() const;
    DataGridColumn* getColumnByIndex(int index);
    DataGridColumn* getColumnByRole(QString role);
    DataGridItemLayout* itemLayout() const;
    int currentIndex() const;
    int getColumnIndex(DataGridColumn* column);
    int itemHeight() const;
    int rowCount() const;
    QAbstractItemModel* model() const;
    QList<DataGridColumn*> columns() const;
    qreal headerHeight() const;
    qreal layoutWidth() const;
    QSortFilterProxyModel* sortFilterProxyModel() const;
    QString alternativeRowBackground() const;
    QString headerBackground() const;
    QString highlightColor() const;
    QString inactiveHighlightColor() const;
    QString observablePropertyName() const;
    Qt::CaseSensitivity filterCaseSensitivity() const;
    SelectionMode selectionMode() const;
    void decrementCurrentIndex();
    void incrementCurrentIndex();
    void selectRow(int row, QMouseEvent *event = NULL);

private:
    DataGridRowPresenter* popItemFromPool();
    int getFirstDisplayedIndex();
    int getLastDisplayedIndex();
    int getRowOffset(int row);
    QPair<int, int> displayedRowRange();
    void alignRow(int row);
    void alignRowToBottom(int row);
    void alignRowToTop(int row);
    void clearRows();
    void populateRows();
    void pushItemToPool(DataGridRowPresenter* item);
    void updateScrollBar();

private slots:
    void modelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int>());
    void modelLayoutChanged(const QList<QPersistentModelIndex> &parents = QList<QPersistentModelIndex>(), QAbstractItemModel::LayoutChangeHint hint = QAbstractItemModel::NoLayoutChangeHint);
    void modelReset();
    void modelRowsInserted(const QModelIndex &parent, int first, int last);
    void modelRowsMoved(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int row);
    void modelRowsRemoved(const QModelIndex &parent, int first, int last);
    void rowHeightChanged();

public slots:
    bool isRowSelected(int row);
    QVariant getUnfilteredValue(int rowIndex, QString role);
    virtual bool filterAcceptsRowHandler(int source_row, const QModelIndex &source_parent);
    virtual QObject* getObservableObject(int row);
    void setAlternativeRowBackground(QString alternativeRowBackground);
    void setBackgroundEnabled(bool backgroundEnabled);
    void setCurrentIndex(int currentIndex, bool align = true);
    void setFilterCaseSensitivity(Qt::CaseSensitivity filterCaseSensitivity);
    void setHeaderBackground(QString headerBackground);
    void setHighlightColor(QString highlightColor);
    void setInactiveHighlightColor(QString inactiveHighlightColor);
    void setIsReadOnly(bool isReadOnly);
    void setItemHeight(int itemHeight);
    void setModel(QAbstractItemModel *model);
    void setObservablePropertyName(QString observablePropertyName);
    void setSelectionMode(SelectionMode selectionMode);
    void setSortEnabled(bool sortEnabled);
    void updateVisibleRange(bool refreshVisible = false);

signals:
    void alternativeRowBackgroundChanged();
    void backgroundEnabledChanged(bool backgroundEnabled);
    void columnsChanged(QList<DataGridColumn*> columns);
    void currentIndexChanged(int currentIndex);
    void filterAcceptsRow(FilterAcceptsRowEventArgs* eventArgs);
    void headerBackgroundChanged();
    void headerHeightChanged();
    void highlightColorChanged();
    void inactiveHighlightColorChanged();
    void isReadOnlyChanged(bool isReadOnly);
    void itemHeightChanged(int itemHeight);
    void layoutWidthChanged();
    void modelChanged(QAbstractItemModel *model);
    void observableObjectRequest(ObservableObjectEventArgs *arg);
    void observablePropertyNameChanged(QString observablePropertyName);
    void rowCountChanged();
    void selectionChanged();
    void selectionModeChanged(SelectionMode selectionMode);
    void sortEnabledChanged(bool sortEnabled);
    void userEvent(QString eventName, QVariant value);

private:
    bool m_backgroundEnabled;
    bool m_isReadOnly;
    bool m_sortEnabled;
    bool m_updateScrollBar;
    DataGridHeaderPresenter *m_header;
    DataGridItemLayout* m_itemLayout;
    DataGridSortFilterProxyModel* m_sortFilterProxyModel;
    int m_currentIndex;
    int m_itemHeight;
    QAbstractItemModel* m_model;
    QList<DataGridColumn*> m_columns;
    QList<int> m_items;
    QMap<int, DataGridRowPresenter*> m_displayedItems;
    QQueue<DataGridRowPresenter*> m_itemsPool;
    QQuickItem* m_layout;
    QQuickItem* m_scrollBar;
    QString m_alternativeRowBackground;
    QString m_headerBackground;
    QString m_highlightColor;
    QString m_inactiveHighlightColor;
    QString m_observablePropertyName;
    QTimer* m_updateTimer;
    SelectionMode m_selectionMode;

    // QQmlParserStatus interface
public:
    void componentComplete() override;

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // DATAGRID_H
