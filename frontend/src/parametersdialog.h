#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QFormLayout>
#include <QPushButton>
#include <QDoubleValidator>

class ParametersDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ParametersDialog(QWidget *parent = nullptr);

    // Геттеры
    double getAlpha() const;
    double getBeta() const;
    double getGamma() const;
    double getSigma() const;
    double getTolerance() const;
    QString getStartPoint() const;

    // Сеттеры
    void setAlpha(double value);
    void setBeta(double value);
    void setGamma(double value);
    void setSigma(double value);
    void setTolerance(double value);
    void setStartPoint(const QString& value);

private slots:
    void resetToDefaults();

private:
    QLineEdit *alphaEdit;
    QLineEdit *betaEdit;
    QLineEdit *gammaEdit;
    QLineEdit *sigmaEdit;
    QLineEdit *toleranceEdit;
    QLineEdit *startPointEdit;
};
