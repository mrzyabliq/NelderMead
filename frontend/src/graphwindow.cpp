#include "graphwindow.h"
#include <QPainter>
#include <QPainterPath>
#include <algorithm>
#include <cmath>

GraphWindow::GraphWindow(QWidget *parent)
    : QWidget(parent, Qt::Window),
      m_title("График сходимости"),
      m_xLabel("Итерации"),
      m_yLabel("Значение функции"),
      m_minValue(0),
      m_maxValue(1),
      m_maxIteration(0)
{
    setWindowTitle(m_title);
    resize(800, 600);
    setAttribute(Qt::WA_DeleteOnClose);
}

void GraphWindow::setHistoryData(const std::vector<double>& history)
{
    m_history = history;
    calculateBounds();
    update();
}

void GraphWindow::setTitle(const QString& title)
{
    m_title = title;
    setWindowTitle(title);
    update();
}

void GraphWindow::setAxisLabels(const QString& xLabel, const QString& yLabel)
{
    m_xLabel = xLabel;
    m_yLabel = yLabel;
    update();
}

void GraphWindow::calculateBounds()
{
    if (m_history.empty()) {
        m_minValue = 0;
        m_maxValue = 1;
        m_maxIteration = 0;
        return;
    }

    auto [minIt, maxIt] = std::minmax_element(m_history.begin(), m_history.end());
    m_minValue = *minIt;
    m_maxValue = *maxIt;
    m_maxIteration = static_cast<int>(m_history.size()) - 1;

    double range = m_maxValue - m_minValue;
    if (range < 1e-10) {
        range = 1.0;
        m_minValue -= 0.5;
        m_maxValue += 0.5;
    } else {
        m_minValue -= range * 0.1;
        m_maxValue += range * 0.1;
    }
}

void GraphWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), Qt::white);
    
    if (m_history.empty()) {
        painter.drawText(rect(), Qt::AlignCenter, "Нет данных для отображения");
        return;
    }

    const int margin = 60;
    QRect plotArea = rect().adjusted(margin, margin, -margin, -margin);
    
    drawGrid(painter, plotArea);
    drawCurve(painter, plotArea);
    
    painter.setPen(Qt::black);
    QFont titleFont = font();
    titleFont.setBold(true);
    titleFont.setPointSize(12);
    painter.setFont(titleFont);
    painter.drawText(rect().adjusted(0, 10, 0, 0), 
                   Qt::AlignHCenter | Qt::AlignTop, 
                   m_title);
}

void GraphWindow::drawGrid(QPainter &painter, const QRect &plotArea)
{
    painter.setPen(QPen(Qt::black, 2));
    painter.drawLine(plotArea.bottomLeft(), plotArea.bottomRight());
    painter.drawLine(plotArea.bottomLeft(), plotArea.topLeft());
    
    painter.drawText(plotArea.bottomRight() + QPoint(10, 0), m_xLabel);
    painter.drawText(plotArea.topLeft() + QPoint(-50, 20), m_yLabel);
    
    int xSteps = std::min(10, m_maxIteration);
    for (int i = 0; i <= xSteps; ++i) {
        int x = plotArea.left() + (i * plotArea.width()) / xSteps;
        painter.drawLine(x, plotArea.bottom(), x, plotArea.bottom() + 5);
        painter.drawText(QRect(x - 20, plotArea.bottom() + 10, 40, 20),
                       Qt::AlignCenter,
                       QString::number(i * m_maxIteration / xSteps));
    }
    
    int ySteps = 8;
    for (int i = 0; i <= ySteps; ++i) {
        double value = m_minValue + (m_maxValue - m_minValue) * i / ySteps;
        int y = plotArea.bottom() - (i * plotArea.height()) / ySteps;
        
        painter.drawLine(plotArea.left() - 5, y, plotArea.left(), y);
        painter.drawText(QRect(plotArea.left() - 50, y - 10, 45, 20),
                       Qt::AlignRight | Qt::AlignVCenter,
                       QString::number(value, 'g', 4));
    }
}

void GraphWindow::drawCurve(QPainter &painter, const QRect &plotArea)
{
    if (m_history.size() < 2) return;

    double xStep = (m_maxIteration > 0) ? plotArea.width() / static_cast<double>(m_maxIteration) : 0;
    double yScale = (m_maxValue - m_minValue > 1e-10) ? plotArea.height() / (m_maxValue - m_minValue) : 1.0;

    QPainterPath path;
    QPen graphPen(QColor(65, 105, 225), 2);
    painter.setPen(graphPen);

    for (size_t i = 0; i < m_history.size(); ++i) {
        double x = plotArea.left() + i * xStep;
        double y = plotArea.bottom() - (m_history[i] - m_minValue) * yScale;
        
        if (i == 0) path.moveTo(x, y);
        else path.lineTo(x, y);
    }
    painter.drawPath(path);

    painter.setBrush(Qt::white);
    int pointStep = std::max(1, static_cast<int>(m_history.size() / 20));
    
    for (size_t i = 0; i < m_history.size(); i += pointStep) {
        double x = plotArea.left() + i * xStep;
        double y = plotArea.bottom() - (m_history[i] - m_minValue) * yScale;
        painter.drawEllipse(QPointF(x, y), 4, 4);
    }
}