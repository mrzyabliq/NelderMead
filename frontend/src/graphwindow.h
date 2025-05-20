#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

#include <QWidget>
#include <vector>

class GraphWindow : public QWidget
{
    Q_OBJECT
public:
    explicit GraphWindow(QWidget *parent = nullptr);
    ~GraphWindow() override = default;

    void setHistoryData(const std::vector<double>& history);
    void setTitle(const QString& title);
    void setAxisLabels(const QString& xLabel, const QString& yLabel);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void calculateBounds();
    void drawGrid(QPainter &painter, const QRect &plotArea);
    void drawCurve(QPainter &painter, const QRect &plotArea);

    std::vector<double> m_history;
    QString m_title;
    QString m_xLabel;
    QString m_yLabel;
    double m_minValue;
    double m_maxValue;
    int m_maxIteration;
};

#endif // GRAPHWINDOW_H