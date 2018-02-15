#ifndef VENTANAPRINCIPAL_H
#define VENTANAPRINCIPAL_H

#include <QMainWindow>
#include "formayuda.h"
//#include "yaml-cpp/yaml.h"
//#include "parser.h"


namespace Ui {
class VentanaPrincipal;
}

class VentanaPrincipal : public QMainWindow
{
    Q_OBJECT

public:
    explicit VentanaPrincipal(QWidget *parent = 0);

    ~VentanaPrincipal();

private slots:
    void on_actionAbrir_YAML_triggered();

    void on_actionGuardar_triggered();

    void on_actionCerrar_triggered();

    void on_botonComprobacion_clicked();

    void on_exportarGro_clicked();

    void on_botonTraducir_clicked(); 

    void on_actionHelp_triggered();

private:
    Ui::VentanaPrincipal *ui;
    void guardarArchivo();
    void escribirEnGro();
    int metodoSimulation(int);
    int metodoSignals(int);
    int metodoGenetics(int);
    int metodoStrain(int);
    int metodoCellActions(int);
    int metodoWorldActions(int);
    int metodoOutput(int);
    bool validaNumeroFloat(QString);
    bool validaNumeroInt(QString);
    QStringList linesGro;
    QStringList linesYaml;
    QString numeroString;
    QString curFile;
    QString path;
    QString textoPonerEnGro;
    bool textoCorrecto;
    int numeroLineaDeYaml;
    int numeroLineaGro;
    int contCepas;
    int numGeneticsElement;
    int numPlasmid;
    std::string rutaFiche;
    QStringList nombreDeTodosLosElementos;
    QStringList nCepas;
    QStringList nomCepas;
    QStringList plCepas;
    QStringList radCrecCep;
    QStringList cepas;
    QStringList program;
    QStringList nombrePopul;
    QStringList centroCirculo1;
    QStringList centroCirculo2;
    QStringList radioCircul;
    QStringList auPl2;
    QStringList auGr;
    QStringList nombreGlobalCepa;
    QStringList nombreGlobalSignal;
    bool boolSimulations;
    bool boolSignals;
    bool boolGenetics;
    bool boolStrains;
    bool boolCellActions;
    bool boolWorldActions;
    bool boolOutput;
    bool textoSemanticoCorrecto;
    QStringList errorSemantico;
};

#endif // VENTANAPRINCIPAL_H
