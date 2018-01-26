#include "ventanaprincipal.h"
#include "formayuda.h"
#include "ui_ventanaprincipal.h"
#include "ui_formayuda.h"
#include <QTextStream>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QString>
#include <QStringList>
#include <iostream>
#include <QDebug>
#include <QLabel>
#include <fstream>
#include <QPlainTextEdit>
#include <QInputMethod>
#include <iostream>
#include <fstream>
//#include <yaml-cpp/yaml.h>
#include <fstream>

VentanaPrincipal::VentanaPrincipal(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VentanaPrincipal)
{
    //inicializar botones no habilitados y texto yaml y texto gro vacios
    ui->setupUi(this);
    connect(ui->actionHelp,SIGNAL(clicked()), this,SLOT(on_actionHelp_triggered()));
    ui->botonTraducir->setEnabled(false);
    ui->botonComprobacion->setEnabled(false);
    ui->textoGro->setEnabled(false);
    ui->exportarGro->setEnabled(false);
}
using namespace std;

VentanaPrincipal::~VentanaPrincipal()
{
    delete ui;
}
//muestra el cuadro de ayuda
void VentanaPrincipal::on_actionHelp_triggered()
{
    Ui::FormAyuda ayuda;
    QWidget *widget = new QWidget;
    ayuda.setupUi(widget);
    widget->show();
}
//accion que se ejecuta al pulsar el boton abrir para mostrar un fichero yaml en la interfaz
void VentanaPrincipal::on_actionAbrir_YAML_triggered()
{
    //guardamos en path la ruta del fichero que hemos abierto
    path = QFileDialog::getOpenFileName(this, tr("Open file"), ".", tr("YAML files(*.yaml)"));
    if(path.length() == 0)
    {
        QMessageBox::information(NULL, tr("Path"), tr("You did not select any file."));
    }
    qDebug() << "esto es path "+path;
    //limpiamos todas las varibles cada vez que abrimos un archivo
    linesYaml.clear();
    linesGro.clear();
    errorSemantico.clear();
    boolSimulations = false;
    boolSignals = false;
    boolGenetics = false;
    boolStrains = false;
    boolCellActions = false;
    boolWorldActions = false;
    boolOutput = false;
    textoSemanticoCorrecto = true;
    //asignamos QFile al archivo y establecemos el estado de los botones y de los textos
    QFile archivoYml(path);
    QStringList prueb;
    QString pru = "";
    QTextStream s(&archivoYml);
    int i = 1;
    QString numero = "1";
    if(archivoYml.open(QIODevice::ReadWrite))
    {
        while(true)
        {
            numero = QString::number(i);
            pru = s.readLine();
            if(pru.isNull())
                break;
            if(pru.contains("\n"))
            {
                pru = pru.replace("\n","");
            }
            if(pru.isEmpty() == false)
            {
                if(numero.length() == 1)
                {
                    prueb.append("   "+numero+" -->  "+pru+"\n");
                    i++;
                }
                else if(numero.length() == 2)
                {
                    prueb.append("  "+numero+" -->  "+pru+"\n");
                    i++;
                }
                else if(numero.length() == 3)
                {
                    prueb.append(" "+numero+" -->  "+pru+"\n");
                    i++;
                }
                else
                {
                    prueb.append(""+numero+" -->  "+pru+"\n");
                    i++;
                }
            }
        }
        pru = "";
        for(int i = 0; i < prueb.size(); i++)
        {
            pru = pru + prueb[i];
        }
        ui->textoyaml->setPlainText(pru);
        //ui ->textoyaml->setPlainText(archivoYml.readAll());
        ui->botonComprobacion->setEnabled(true);
        ui->textoGro->setEnabled(true);
        ui->exportarGro->setEnabled(true);
        ui->labelYamlCorrecto->setText("");
        ui->textoGro->setEnabled(false);
        ui->textoGro->setPlainText("");
        textoCorrecto = false;
        ui->botonTraducir->setEnabled(false);
        archivoYml.close();
    }   
}
//método que se produce al pulsar el botón guardar. Guarda las modificaciones realizadas en el documento que has abierto
void VentanaPrincipal::on_actionGuardar_triggered()
{
    linesYaml.clear();
    linesGro.clear();
    errorSemantico.clear();  
    QString a = "";
    QStringList aux;
    QStringList aux1;
    aux.clear();
    aux1.clear();
    aux = ui->textoyaml->toPlainText().split("\n");
    for(int i = 0; i < aux.size(); i++)
    {
        aux1.clear();
        aux1 = aux[i].split(" -->  ");
        if(aux1.size() == 2)
            aux[i] = aux1[1]+"\n";
    }
    a = "";
    for(int i = 0; i < aux.size(); i++)
    {
        a = a + aux[i];
    }
    QFile archivoYml(path);
    archivoYml.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream s(&archivoYml);
    qDebug() << "ha pasado el bucle while true";
    qDebug() << "pasa el for y a es "+a;
    s<<a;

    qDebug() << "pasa s<<a";
    archivoYml.close();
    ui->labelYamlCorrecto->setText("");
    ui->textoGro->setEnabled(false);
    ui->textoGro->setPlainText("");
    textoCorrecto = false;
    textoSemanticoCorrecto = true;
}
//metodo que cierra el menu y pone en texto yaml en blanco
void VentanaPrincipal::on_actionCerrar_triggered()
{
    ui->textoyaml->setPlainText("");
}
//método que comprueba si el texto es yaml correcto, se hace con la libreria yaml-cpp
void VentanaPrincipal::on_botonComprobacion_clicked()
{
    QFile archivoYml(path);
    QString rutaArchivo = path;
    //falta implementacion en este metdo porque he tenido problemas con la libreria en windows
    archivoYml.close();
    linesYaml.clear();
    linesGro.clear();
    textoCorrecto = true;
    if(textoCorrecto)
    {
        ui->labelYamlCorrecto->setText("Correct");
        ui->botonTraducir->setEnabled(true);
        ui->labelGro->setText("Text in Gro");

    }
    else
    {
        ui->labelYamlCorrecto->setText("Incorrect");
        ui->botonTraducir->setEnabled(false);
        ui->labelGro->setText("Errors detected in yaml file");
    }
}
//metodo que escribe en el cuadro texto gro el contenido de la variable textoPonerEnGro
void VentanaPrincipal::escribirEnGro()
{
    qDebug() << "Entra en escribirEnGro";
    ui->textoGro->clear();
    ui->textoGro->setPlainText(textoPonerEnGro);
}
//metodo para guardar el resultado de la traduccion en .gro
void VentanaPrincipal::guardarArchivo()
{
    QFile archiv(curFile);
    if(archiv.open(QFile::WriteOnly))
    {
        archiv.write(ui->textoGro->toPlainText().toUtf8());
    }
    else
    {
        QMessageBox::warning(this, "textoGro", tr("Cannot write file %1.\nError: %2").arg(curFile).arg(archiv.errorString()));
    }
    archiv.close();
}
//metodo para guardar el resultado de la traduccion en .gro en la ruta especificada
void VentanaPrincipal::on_exportarGro_clicked()
{
    QString nombreArchivo = QFileDialog::getSaveFileName(this,"textGro - Save as","Ejemplo","Gro Files (*.gro)");
    if(!nombreArchivo.isEmpty())
    {
        curFile = nombreArchivo;
        guardarArchivo();
    }
}
//metodo que valida si el QString que le pases es un numero tipo float
bool VentanaPrincipal::validaNumeroFloat(QString numero)
{
    QStringList aux;
    bool valida = true;
    if(numero.contains(".") == false)
    {
        int tam = numero.size();
        for(int i = 0; i < tam; i++)
        {
            if(numero[i] < 48 || numero[i] > 57)
            {
                valida = false;
                break;
            }
        }
    }
    else
    {
        aux = numero.split(".");
        qDebug() << aux.size();
        if(aux.size() != 2)
        {
            valida = false;
        }
        else
        {
            if(validaNumeroInt(aux[0]) == false || validaNumeroInt(aux[1]) == false)
            {
                valida = false;
            }
        }
    }
    return valida;
}
//metodo que valida si el QString que le pases es un numero tipo int
bool VentanaPrincipal::validaNumeroInt(QString numero)
{
    bool valida = true;
    int tam = numero.size();
    for(int i = 0; i < tam; i++)
    {
        if(numero[i] < 48 || numero[i] > 57)
        {
            valida = false;
            break;
        }
    }
    return valida;
}
//metodo que se encarga de traducir la parte de simulation
int VentanaPrincipal::metodoSimulation(int lineaActual)
{
    qDebug() << "entra en el metodo simulation "+linesYaml[lineaActual];
    numeroLineaDeYaml = lineaActual;
    numeroLineaDeYaml++;//pasa a la siguiente linea despues de simulation
    int ultiDt = 0;
    int ultiSeed = 0;
    int ultiPopu = 0;
    qDebug() << "linea siguiente"+linesYaml[numeroLineaDeYaml];
    //bucle que recorre las lineas de simulation hasta llegar a la siguiente parte
    while(linesYaml[numeroLineaDeYaml].contains("signals") == false && linesYaml[numeroLineaDeYaml].contains("genetics") == false && linesYaml[numeroLineaDeYaml].contains("strains") == false && linesYaml[numeroLineaDeYaml].contains("cell") == false && linesYaml[numeroLineaDeYaml].contains("world") == false && linesYaml[numeroLineaDeYaml].contains("output") == false)
    {
        //comprobacion de que las lineas tienen dt, seed o max_population cualquier otro argumento dar error
        if(linesYaml[numeroLineaDeYaml].contains("dt:") == false && linesYaml[numeroLineaDeYaml].contains("seed:") == false && linesYaml[numeroLineaDeYaml].contains("max_population:") == false)
        {
            numeroString = QString::number(numeroLineaDeYaml+1);
            textoSemanticoCorrecto = false;
            errorSemantico.append("\nERROR line "+numeroString+": Invalid simulation value. Valid simulation values are only \"dt\",\"seed\" and \"max_population\"");
        }
        //guardar los valores de de, seed y max_population en variables para su posterior utilizacion
        if(linesYaml[numeroLineaDeYaml].contains("dt"))
        {
            linesYaml[numeroLineaDeYaml].replace("dt:","");
            ultiDt = numeroLineaDeYaml;
        }
        if(linesYaml[numeroLineaDeYaml].contains("seed"))
        {
            linesYaml[numeroLineaDeYaml].replace("seed:","");
            ultiSeed = numeroLineaDeYaml;
        }
        if(linesYaml[numeroLineaDeYaml].contains("max_population"))
        {
            linesYaml[numeroLineaDeYaml].replace("max_population:","");
            ultiPopu = numeroLineaDeYaml;
        }
        //avanzar a la siguiente linea
        numeroLineaDeYaml++;
        qDebug() << "linea en el bucle "+linesYaml[numeroLineaDeYaml];
    }
    //comprobar si el valor de dt es tipo float
    if(validaNumeroFloat(linesYaml[ultiDt]))//si esta bien traducir
    {
        linesGro.append("\nset(\"dt\", "+linesYaml[ultiDt]+");\n");
    }
    else
    {
        numeroString = QString::number(ultiDt+1);
        textoSemanticoCorrecto = false;
        errorSemantico.append("\nERROR line "+numeroString+": the dt's value has to be a number(float)\n");
    }
    //comprobar si el valor de seed es de tipo int
    if(validaNumeroInt(linesYaml[ultiSeed]))//si esta bien traducir
    {
        linesGro.append("\nset(\"seed\", "+linesYaml[ultiSeed]+");\n");
    }
    else
    {
        numeroString = QString::number(ultiSeed+1);
        textoSemanticoCorrecto = false;
        errorSemantico.append("\nERROR line "+numeroString+": the seed's value has to be a number(int)\n");
    }
    //comprobar si el valor de max_population es de tipo int
    if(validaNumeroInt(linesYaml[ultiPopu]))//si esta bien traducir
    {
        linesGro.append("\nset(\"population_max\", "+linesYaml[ultiPopu]+");\n");
    }
    else
    {
        numeroString = QString::number(ultiPopu+1);
        textoSemanticoCorrecto = false;
        errorSemantico.append("\nERROR line "+numeroString+": the max_population's value has to be a number(int)\n");
    }
    qDebug() << "Va a salir del simulation";
    return numeroLineaDeYaml;
}
//metodo que se encarga de traducir la parte de signals
int VentanaPrincipal::metodoSignals(int lineaActual)
{
    numeroLineaDeYaml = lineaActual + 1;//avanza una linea mas
    int lineatype = 0;
    int lineadiffusion = 0;
    int lineaneighbors = 0;
    int contsignals = 0;
    int nomSignals = 0;
    int elemDif = 0;
    int elemDeg = 0;
    int elemInit = 0;
    qDebug() << "linea164 "+linesYaml[numeroLineaDeYaml];
    //bucle para recorrer todas las lineas de signals hasta llegar a la siguiente parte
    while(linesYaml[numeroLineaDeYaml].contains("simulations") == false && linesYaml[numeroLineaDeYaml].contains("genetics") == false && linesYaml[numeroLineaDeYaml].contains("strains") == false && linesYaml[numeroLineaDeYaml].contains("cell") == false && linesYaml[numeroLineaDeYaml].contains("world") == false && linesYaml[numeroLineaDeYaml].contains("output") == false)
    {
        //parte de grid
        if(linesYaml[numeroLineaDeYaml].contains("grid"))
        {
            numeroLineaDeYaml = numeroLineaDeYaml + 1;
            qDebug() << "linea170 "+linesYaml[numeroLineaDeYaml];
            //bucle para recorrer todas las lineas de grid hasta llegar a la siguiente parte
            while(linesYaml[numeroLineaDeYaml].contains("element") == false && linesYaml[numeroLineaDeYaml].contains("simulations") == false && linesYaml[numeroLineaDeYaml].contains("genetics") == false && linesYaml[numeroLineaDeYaml].contains("strains") == false && linesYaml[numeroLineaDeYaml].contains("cell") == false && linesYaml[numeroLineaDeYaml].contains("world") == false && linesYaml[numeroLineaDeYaml].contains("output") == false)
            {
                //comprobar si hay argumentos que no estan especificados
                if(linesYaml[numeroLineaDeYaml].contains("type:") == false && linesYaml[numeroLineaDeYaml].contains("diffusion_method:") == false && linesYaml[numeroLineaDeYaml].contains("neighbors:") == false)
                {
                    numeroString = QString::number(numeroLineaDeYaml+1);
                    textoSemanticoCorrecto = false;
                    errorSemantico.append("\nERROR line "+numeroString+": Invalid signal grid value. Valid simulation values are only \"type\",\"diffusion_method\" and \"neighbors\"\n");
                }
                //guardar los argumentos que estan especificados
                if(linesYaml[numeroLineaDeYaml].contains("type"))
                {
                    linesYaml[numeroLineaDeYaml].replace("type:","");
                    if((linesYaml[numeroLineaDeYaml].replace("type:","").compare("continuous") == 0|| linesYaml[numeroLineaDeYaml].replace("type:","").compare("discrete") == 0)==false)
                    {
                        numeroString = QString::number(numeroLineaDeYaml+1);
                        textoSemanticoCorrecto = false;
                        errorSemantico.append("\nERROR line "+numeroString+": Invalid element type value. Valid types are only \"continuous\" and \"discrete\"\n");
                    }
                    lineatype = numeroLineaDeYaml;
                }
                if(linesYaml[numeroLineaDeYaml].contains("diffusion"))
                {
                    linesYaml[numeroLineaDeYaml].replace("diffusion_method:","");
                    lineadiffusion = numeroLineaDeYaml;
                }
                if(linesYaml[numeroLineaDeYaml].contains("neigh"))
                {
                    linesYaml[numeroLineaDeYaml].replace("neighbors:","");
                    if(validaNumeroInt(linesYaml[numeroLineaDeYaml]) == false)
                    {
                        numeroString = QString::number(numeroLineaDeYaml+1);
                        textoSemanticoCorrecto = false;
                        errorSemantico.append("\nERROR line "+numeroString+": the neighbors' value has to be a number(int)\n");
                    }
                    lineaneighbors = numeroLineaDeYaml;
                }
                numeroLineaDeYaml++;
            }
            //comprobar si los argumentos son del tipo correcto en el caso de que esten definidos
            if(lineatype+lineadiffusion+lineaneighbors == 0)
            {
                linesGro.append("\ngrid ( \"continuous\", \"gro_original\", 10, 10, 8);\n");
            }
            else if(lineatype+lineadiffusion == 0)
            {
                if(validaNumeroInt(linesYaml[lineaneighbors]))
                {
                    linesGro.append("\ngrid ( \"continuous\", \"gro_original\", 10, 10, "+linesYaml[lineaneighbors]+");\n");
                }

            }
            else if(lineatype+lineaneighbors == 0)
            {
                linesGro.append("\ngrid ( \"continuous\", \""+linesYaml[lineadiffusion]+"\", 10, 10, 8);\n");
            }
            else if(lineadiffusion+lineaneighbors == 0)
            {
                if(linesYaml[lineatype] == "continuous" || linesYaml[lineatype] == "discrete")
                {
                    linesGro.append("\ngrid ( \""+linesYaml[lineatype]+"\", \"gro_original\", 10, 10, 8);\n");
                }

            }
            else if(lineatype == 0)
            {
                if(validaNumeroInt(linesYaml[lineaneighbors]))
                {
                    linesGro.append("\ngrid ( \"continuous\", \""+linesYaml[lineadiffusion]+"\", 10, 10, "+linesYaml[lineaneighbors]+");\n");
                }


            }
            else if(lineadiffusion == 0)
            {
                if((linesYaml[lineatype] == "continuous" || linesYaml[lineatype] == "discrete") && validaNumeroInt(linesYaml[lineaneighbors]))
                {
                    linesGro.append("\ngrid ( \""+linesYaml[lineatype]+"\", \"gro_original\", 10, 10, "+linesYaml[lineaneighbors]+");\n");
                }

            }
            else if(lineaneighbors == 0)
            {
                if(linesYaml[lineatype] == "continuous" || linesYaml[lineatype] == "discrete")
                {
                    linesGro.append("\ngrid ( \""+linesYaml[lineatype]+"\", \""+linesYaml[lineadiffusion]+"\", 10, 10, 8);\n");
                }


            }
            else
            {
                if((linesYaml[lineatype] == "continuous" || linesYaml[lineatype] == "discrete") && validaNumeroInt(linesYaml[lineaneighbors]))
                {
                    linesGro.append("\ngrid ( \""+linesYaml[lineatype]+"\", \""+linesYaml[lineadiffusion]+"\", 10, 10, "+linesYaml[lineaneighbors]+");\n");
                }

            }
        }
        //parte de element
        qDebug() << "linea227 "+linesYaml[numeroLineaDeYaml];
        if(linesYaml[numeroLineaDeYaml].contains("element"))
        {
            numeroLineaDeYaml = numeroLineaDeYaml + 1;
            qDebug() << "linea231 "+linesYaml[numeroLineaDeYaml];
            //bucle para recorrer todas las lineas de element hasta llegar a la siguiente parte
            while(linesYaml[numeroLineaDeYaml].contains("grid") == false && linesYaml[numeroLineaDeYaml].contains("simulations") == false && linesYaml[numeroLineaDeYaml].contains("genetics") == false && linesYaml[numeroLineaDeYaml].contains("strains") == false && linesYaml[numeroLineaDeYaml].contains("cell") == false && linesYaml[numeroLineaDeYaml].contains("world") == false && linesYaml[numeroLineaDeYaml].contains("output") == false)
            {
                //como el nombre es obligatorio si no lo hay error
                if(linesYaml[numeroLineaDeYaml].contains("name") == false)
                {
                    numeroString = QString::number(numeroLineaDeYaml+1);
                    textoSemanticoCorrecto = false;
                    errorSemantico.append("\nERROR line "+numeroString+": the first argument in signals, in elements, has to be \"name\"\n");
                    numeroLineaDeYaml++;
                }
                //guardar todos los argumentos para poder luego traducirlos
                else if(linesYaml[numeroLineaDeYaml].contains("name"))
                {
                    contsignals++;
                    linesYaml[numeroLineaDeYaml].replace("-name:","");
                    nomSignals = numeroLineaDeYaml;
                    nombreGlobalSignal.append(linesYaml[nomSignals]);
                    numeroLineaDeYaml++;
                    while(linesYaml[numeroLineaDeYaml].contains("name") == false && linesYaml[numeroLineaDeYaml].contains("genetics") == false && linesYaml[numeroLineaDeYaml].contains("strains") == false && linesYaml[numeroLineaDeYaml].contains("cell") == false && linesYaml[numeroLineaDeYaml].contains("world") == false && linesYaml[numeroLineaDeYaml].contains("output") == false)
                    {
                        if(linesYaml[numeroLineaDeYaml].contains("diffusion"))
                        {
                            linesYaml[numeroLineaDeYaml].replace("diffusion:","");
                            elemDif = numeroLineaDeYaml;
                        }
                        if(linesYaml[numeroLineaDeYaml].contains("degradation"))
                        {
                            linesYaml[numeroLineaDeYaml].replace("degradation:","");
                            elemDeg = numeroLineaDeYaml;
                        }
                        if(linesYaml[numeroLineaDeYaml].contains("init"))
                        {
                            linesYaml[numeroLineaDeYaml].replace("init_value:","");
                            elemInit = numeroLineaDeYaml;
                        }
                        numeroLineaDeYaml++;
                        qDebug() << "linea258 "+linesYaml[numeroLineaDeYaml];
                    }
                    //comprobar que los argumentos son del tipo dado
                    //los que no estan definidos se ponen los valores por defecto
                    if(elemDeg+elemDif+elemInit == 0)
                    {
                        linesGro.append("\n"+linesYaml[nomSignals]+" := s_signal([kdiff := 0.02, kdeg := 0.005, init_value : = 0]);\n");
                    }
                    else if(elemDeg+elemDif == 0)
                    {
                        if(validaNumeroFloat(linesYaml[elemInit]))
                        {
                            linesGro.append("\n"+linesYaml[nomSignals]+" := s_signal([kdiff := 0.02, kdeg := 0.005, init_value : = "+linesYaml[elemInit]+"]);\n");
                        }
                        else
                        {
                            numeroString = QString::number(elemInit+1);
                            textoSemanticoCorrecto = false;
                            errorSemantico.append("\nERROR line "+numeroString+": in signals, in elements, init_value has to be a number (float)\n");
                        }
                    }
                    else if(elemDif+elemInit == 0)
                    {
                        if(validaNumeroFloat(linesYaml[elemDeg]))
                        {
                            linesGro.append("\n"+linesYaml[nomSignals]+" := s_signal([kdiff := 0.02, kdeg := "+linesYaml[elemDeg]+", init_value : = 0]);\n");
                        }
                        else
                        {
                            numeroString = QString::number(elemDeg+1);
                            textoSemanticoCorrecto = false;
                            errorSemantico.append("\nERROR line "+numeroString+": in signals, in elements, degradation has to be a number (float)\n");
                        }

                    }
                    else if(elemDeg+elemInit == 0)
                    {

                        if(validaNumeroFloat(linesYaml[elemDif]))
                        {
                            linesGro.append("\n"+linesYaml[nomSignals]+" := s_signal([kdiff := "+linesYaml[elemDif]+", kdeg := 0.005, init_value : = 0]);\n");
                        }
                        else
                        {
                            numeroString = QString::number(elemDif+1);
                            textoSemanticoCorrecto = false;
                            errorSemantico.append("\nERROR line "+numeroString+": in signals, in elements, diffusion has to be a number (float)\n");
                        }
                    }
                    else if(elemDif == 0)
                    {
                        if(validaNumeroFloat(linesYaml[elemDeg]) && validaNumeroFloat(linesYaml[elemInit]))
                        {
                            linesGro.append("\n"+linesYaml[nomSignals]+" := s_signal([kdiff := 0.02, kdeg := "+linesYaml[elemDeg]+", init_value : = "+linesYaml[elemInit]+"]);\n");
                        }
                        else
                        {
                            numeroString = QString::number(elemDeg+1)+" or line "+QString::number(elemInit+1);
                            textoSemanticoCorrecto = false;
                            errorSemantico.append("\nERROR line "+numeroString+": in signals, in elements, degradation and init_value have to be a number (float)\n");
                        }

                    }
                    else if(elemDeg == 0)
                    {
                        if(validaNumeroFloat(linesYaml[elemDif]) && validaNumeroFloat(linesYaml[elemInit]))
                        {
                            linesGro.append("\n"+linesYaml[nomSignals]+" := s_signal([kdiff := "+linesYaml[elemDif]+", kdeg := 0.005, init_value : = "+linesYaml[elemInit]+"]);\n");
                        }
                        else
                        {
                            numeroString = QString::number(elemDif+1)+" or line "+QString::number(elemInit+1);
                            textoSemanticoCorrecto = false;
                            errorSemantico.append("\nERROR line "+numeroString+": in signals, in elements, diffusion and init_value have to be a number (float)\n");
                        }

                    }
                    else if(elemInit == 0)
                    {
                        if(validaNumeroFloat(linesYaml[elemDif]) && validaNumeroFloat(linesYaml[elemDeg]))
                        {
                            linesGro.append("\n"+linesYaml[nomSignals]+" := s_signal([kdiff := "+linesYaml[elemDif]+", kdeg := "+linesYaml[elemDeg]+", init_value : = 0]);\n");
                        }
                        else
                        {
                            numeroString = QString::number(elemDeg+1)+" or line "+QString::number(elemDif+1);
                            textoSemanticoCorrecto = false;
                            errorSemantico.append("\nERROR line "+numeroString+": in signals, in elements, diffusion and degradation have to be a number (float)\n");
                        }
                    }
                    else
                    {
                        if(validaNumeroFloat(linesYaml[elemDif]) && validaNumeroFloat(linesYaml[elemInit]) && validaNumeroFloat(linesYaml[elemDeg]))
                        {
                            linesGro.append("\n"+linesYaml[nomSignals]+" := s_signal([kdiff := "+linesYaml[elemDif]+", kdeg := "+linesYaml[elemDeg]+", init_value : = "+linesYaml[elemInit]+"]);\n");
                        }
                        else
                        {
                            numeroString = QString::number(elemDeg+1)+" or line "+QString::number(elemInit+1)+" or line "+QString::number(elemDif+1);
                            textoSemanticoCorrecto = false;
                            errorSemantico.append("\nERROR line "+numeroString+": in signals, in elements, diffusion, degradation and init_value have to be a number (float)\n");
                        }

                    }
                }
                elemDif = 0;
                elemDeg = 0;
                elemInit = 0;
                qDebug() << "deberia ser name "+linesYaml[numeroLineaDeYaml];
            }
        }
    }
    return numeroLineaDeYaml;
}
//metodo que se encarga de traducir la parte de genetics
int VentanaPrincipal::metodoGenetics(int lineaActual)
{
    numeroLineaDeYaml = lineaActual;
    numeroLineaDeYaml++;
    QStringList nombreElementGenetics;
    QStringList elemType;
    QStringList elemDeg1;
    QStringList elemDeg2;
    QStringList aux;
    qDebug() << "va a hacer element "+linesYaml[numeroLineaDeYaml];
    //guardar los argumentos y partes de element en variables
    //////////       ELEMENT
    if(linesYaml[numeroLineaDeYaml].contains("element"))
    {
        numeroLineaDeYaml++;
        while(linesYaml[numeroLineaDeYaml].contains("operons") == false)
        {
            if(linesYaml[numeroLineaDeYaml].contains("name"))
            {
                linesYaml[numeroLineaDeYaml].replace("-name:","");
                nombreElementGenetics.append(linesYaml[numeroLineaDeYaml]);
                nombreDeTodosLosElementos.append(linesYaml[numeroLineaDeYaml]);
                numeroLineaDeYaml++;
                while(linesYaml[numeroLineaDeYaml].contains("operons") == false && linesYaml[numeroLineaDeYaml].contains("name") == false)
                {
                    if(linesYaml[numeroLineaDeYaml].contains("type"))
                    {
                        elemType.append(linesYaml[numeroLineaDeYaml].replace("type:",""));
                        if(linesYaml[numeroLineaDeYaml].replace("type:","") == "protein" || linesYaml[numeroLineaDeYaml].replace("type:","") == "arn")
                        {

                        }
                        else
                        {
                            numeroString = QString::number(numeroLineaDeYaml+1);
                            textoSemanticoCorrecto = false;
                            errorSemantico.append("\nERROR line "+numeroString+": Invalid element type value. Valid types are only protein and arn.\n");
                        }
                    }
                    if(linesYaml[numeroLineaDeYaml].contains("degradation"))
                    {
                        linesYaml[numeroLineaDeYaml].replace("degradation_time:{mean:","").replace("deviation:","").replace("}","");
                        aux = linesYaml[numeroLineaDeYaml].split(",");
                        elemDeg1.append(aux[0]);
                        elemDeg2.append(aux[1]);
                        if(validaNumeroFloat(aux[0]) == false || validaNumeroFloat(aux[1]) == false)
                        {
                            numeroString = QString::number(numeroLineaDeYaml+1);
                            textoSemanticoCorrecto = false;
                            errorSemantico.append("\nERROR line "+numeroString+": in genetics, in element, in degradation_time, mean and deviation have to be a numers(float)\n ");
                        }
                    }
                    numeroLineaDeYaml++;
                }
            }
        }
    }
    numGeneticsElement = nombreElementGenetics.size();
    //una vez terminado element pasamos a operones
    ////////////       OPERONES
    int f = numeroLineaDeYaml;
    int contOperones = 0;
    //contamos el numero de operones para establecer el tamaño de los arrays donde guardaremos la informacion
    while(linesYaml[f].contains("plasmid") == false)
    {
        if(linesYaml[f].contains("name"))
        {
            contOperones++;
        }
        f++;
    }
    QString nombreOperon[contOperones+1];
    QString promoter[contOperones+1][3][3];
    QString genes[contOperones+1][2][2];
    QString nombreElement1[contOperones+1];
    QString typeElement1[contOperones+1];
    QString degraElement1[contOperones+1][2];
    int contadore = 0;
    //inicializar todos los arrays
    for(int i = 0; i < contOperones+1; i++)
    {
        nombreOperon[i] = "";
        promoter[i][0][0] = "";
        promoter[i][0][1] = "";
        promoter[i][0][2] = "";
        promoter[i][1][0] = "";
        promoter[i][1][1] = "";
        promoter[i][1][2] = "";
        promoter[i][2][0] = "";
        promoter[i][2][1] = "";
        promoter[i][2][2] = "";
        genes[i][0][0] = "";
        genes[i][0][1] = "";
        genes[i][1][0] = "";
        genes[i][1][1] = "";
    }
    qDebug() << "va a hacer operons "+linesYaml[numeroLineaDeYaml];
    if(linesYaml[numeroLineaDeYaml].contains("operons"))
    {
        int it = 0;
        //bucle que recorre las lineas de operons y guardar los atributos en variables
        while(linesYaml[numeroLineaDeYaml].contains("plasmid")==false && linesYaml[numeroLineaDeYaml].contains("strain")==false && linesYaml[numeroLineaDeYaml].contains("cell_actions")==false && linesYaml[numeroLineaDeYaml].contains("world_actions")==false && linesYaml[numeroLineaDeYaml].contains("output")==false)
        {
            qDebug() << "linea 635 "+linesYaml[numeroLineaDeYaml];
            if(linesYaml[numeroLineaDeYaml].contains("name") == false)
            {
                 numeroLineaDeYaml++;
            }
            else if(linesYaml[numeroLineaDeYaml].contains("name"))
            {
                nombreOperon[it] = linesYaml[numeroLineaDeYaml].replace("-name:","");
                it++;
                qDebug() << nombreOperon[it-1]+"este es el nombreOperon[it]";
            }
            qDebug() << "dentro bucle while linea 399"+linesYaml[numeroLineaDeYaml];
            if(linesYaml[numeroLineaDeYaml].contains("promoter"))
            {
                qDebug() << "entra en promoter "+linesYaml[numeroLineaDeYaml];
                while(linesYaml[numeroLineaDeYaml].contains("genes") == false && linesYaml[numeroLineaDeYaml].contains("name") == false && linesYaml[numeroLineaDeYaml].contains("plasmid") == false)
                {
                    numeroLineaDeYaml++;
                    if(linesYaml[numeroLineaDeYaml].contains("gate"))
                    {
                        qDebug() << "entra en gate "+linesYaml[numeroLineaDeYaml];
                        promoter[it-1][0][0] = linesYaml[numeroLineaDeYaml].replace("gate:","");
                        if(promoter[it-1][0][0].compare("TRUE") == 0 || promoter[it-1][0][0].compare("FALSE") == 0 || promoter[it-1][0][0].compare("YES") == 0 || promoter[it-1][0][0].compare("NOT") == 0 || promoter[it-1][0][0].compare("AND") == 0 || promoter[it-1][0][0].compare("OR") == 0 || promoter[it-1][0][0].compare("XOR") == 0 || promoter[it-1][0][0].compare("NAND") == 0)
                        {

                        }
                        else
                        {
                            numeroString = QString::number(numeroLineaDeYaml+1);
                            textoSemanticoCorrecto = false;
                            errorSemantico.append("\nERROR line "+numeroString+": Invalid gate value. Only posible gates are: 'TRUE','FALSE','YES','NOT','AND','OR','NAND','XOR'\n");
                        }
                        qDebug() << "promoter it-1"+promoter[it-1][0][0];
                    }
                    if(linesYaml[numeroLineaDeYaml].contains("transcription"))
                    {
                        qDebug() << "entra en transcription "+linesYaml[numeroLineaDeYaml];
                        promoter[it-1][1][0] = linesYaml[numeroLineaDeYaml].replace("transcription_factors:[","").replace("]","");
                        qDebug() << "promoter it-1"+promoter[it-1][1][0];
                    }
                    if(linesYaml[numeroLineaDeYaml].contains("block"))
                    {
                        qDebug() << "entra en block "+linesYaml[numeroLineaDeYaml];
                        aux.clear();
                        linesYaml[numeroLineaDeYaml].replace("block:{to_on:","").replace("to_off:","").replace("time:","").replace("}","");
                        aux = linesYaml[numeroLineaDeYaml].split(",");
                        promoter[it-1][2][0] = aux[0];
                        qDebug() << "entra en block pro"+promoter[it-1][2][0];
                        promoter[it-1][2][1] = aux[1];
                        qDebug() << "promoter it-1 2 1"+promoter[it-1][2][1];
                        promoter[it-1][2][2] = aux[2];
                        qDebug() << "promoter it-1 2 2"+promoter[it-1][2][2];
                        if(validaNumeroFloat(promoter[it-1][2][0]) == false || validaNumeroFloat(promoter[it-1][2][1]) == false || validaNumeroFloat(promoter[it-1][2][2]) ==false)
                        {
                            numeroString = QString::number(numeroLineaDeYaml+1);
                            textoSemanticoCorrecto = false;
                            errorSemantico.append("\nERROR line "+numeroString+": in genetics, in operons, in promoter, in block, to_on, to_off and time have to be a numers(float)\n ");
                        }
                    }
                }
            }

            if(linesYaml[numeroLineaDeYaml].contains("genes"))
            {
                qDebug() << "entra en genes "+linesYaml[numeroLineaDeYaml];
                while(linesYaml[numeroLineaDeYaml].contains("name") == false && linesYaml[numeroLineaDeYaml].contains("plasmid") == false)
                {
                    numeroLineaDeYaml++;

                    if(linesYaml[numeroLineaDeYaml].contains("expresses"))
                    {
                        qDebug() << "entra en espresses "+linesYaml[numeroLineaDeYaml];
                        genes[it-1][0][0] = linesYaml[numeroLineaDeYaml].replace("-expresses:","");
                        qDebug() << "genes[it-1][0][0] "+genes[it-1][0][0];
                    }
                    if(linesYaml[numeroLineaDeYaml].contains("time"))
                    {
                        qDebug() << "entra en time "+linesYaml[numeroLineaDeYaml];
                        linesYaml[numeroLineaDeYaml].replace("time:{mean:","").replace("deviation:","").replace("}","");
                        aux.clear();
                        aux = linesYaml[numeroLineaDeYaml].split(",");
                        genes[it-1][1][0] = aux[0];
                        genes[it-1][1][1] = aux[1];
                        if(validaNumeroFloat(genes[it-1][1][0]) == false || validaNumeroFloat(genes[it-1][1][1]) == false)
                        {
                            numeroString = QString::number(numeroLineaDeYaml+1);
                            textoSemanticoCorrecto = false;
                            errorSemantico.append("\nERROR line "+numeroString+": in genetics, in operons, int genes, in time, mean and deviation have to be a numers(float)\n ");
                        }
                        qDebug() << "genes[it-1][1][0] "+genes[it-1][1][0];
                        qDebug() << "genes[it-1][1][1] "+genes[it-1][1][1];
                    }
                    qDebug() << "ir viendo que linea es "+linesYaml[numeroLineaDeYaml];
                }
                qDebug() << "linea 713 "+linesYaml[numeroLineaDeYaml];
            }
            qDebug() << "linea 715 "+linesYaml[numeroLineaDeYaml];
        }
        qDebug() << "sale del bucle de opeons "+linesYaml[numeroLineaDeYaml];
        //para la traduccion a gro hay que coincidir el nombre de element con el expresses en la parte de genes de operons
        //coincidir elements con operons
        //y comprobamos en el bucle si son del tipo de dato correcto
        for(int i = 0; i < contOperones; i++)
        {
            qDebug() << "entra aqui 734";     
            for(int j = 0; j < nombreElementGenetics.size(); j++)
            {
                if(genes[i][0][0] == nombreElementGenetics[j])
                {
                    contadore = 1;
                }
            }
            if(contadore == 0)
            {
                numeroString = QString::number(numeroLineaDeYaml+1);
                textoSemanticoCorrecto = false;
                errorSemantico.append("\nERROR: There is a genes->expresses that it doesn't exist in genetics->element\n");
            }
            contadore = 0;
        }
        //si no tiene errores traducimos la parte de genetics, las partes de element y operons
        if(textoSemanticoCorrecto)
        {
            for(int i = 0; i < nombreElementGenetics.size(); i++)
            {
                for(int j = 0; j < nombreElementGenetics.size();j++)
                {
                    qDebug() << genes[i][0][0]+" eso es genes";
                    qDebug() << nombreElementGenetics[j]+" esto es nombre element genetics";
                    if(genes[i][0][0] == nombreElementGenetics[j])
                    {
                        nombreElement1[i] = nombreElementGenetics[j];
                        qDebug() << "nombreElementi "+nombreElement1[i];
                        typeElement1[i] = elemType[j];
                        degraElement1[i][0] = elemDeg1[j];
                        degraElement1[i][1] = elemDeg2[j];
                    }
                }
            }
            qDebug() << "sale del cucle de coincidir "+linesYaml[numeroLineaDeYaml];
            for(int i = 0; i < nombreElementGenetics.size(); i++)
            {
                linesGro.append("\ngenes ( [ name: = \""+nombreOperon[i]+"\", proteins : = { \""+nombreElement1[i]+"\"}"
                                ", promoter : = [ function : = \""+promoter[i][0][0]+"\", transcription_factors := { \""+promoter[i][1][0]+
                                "\"}, noise : = [ toOff := "+promoter[i][2][1]+", to_On : = "+promoter[i][2][0]+", noise_time := "+promoter[i][2][2]+
                                "]], prot_act_times := [ times := {"+genes[i][1][0]+" }, variabilities := { "+genes[i][1][1]+
                                "}], prot_deg_times := [ times := {"+degraElement1[i][0]+" }, variabilities := { "+degraElement1[i][1]+
                                "}]]);\n\n");
            }
            qDebug() << "sale de poner genes ";
        }


    }
    //pasamos dentro de genetics a la parte de plasmidos
    qDebug() << "va a hacer plasmidos "+linesYaml[numeroLineaDeYaml];
    ///////////          PLASMIDOS                 ////////////////////////
    ///
    contadore = 0;
    numPlasmid = 0;
    int a = numeroLineaDeYaml;
    //contamos el numero de plasmidos
    while(linesYaml[a].contains("simulations") == false && linesYaml[a].contains("signals") == false && linesYaml[a].contains("strains") == false && linesYaml[a].contains("cell") == false && linesYaml[a].contains("world") == false && linesYaml[a].contains("output") == false)
    {
        if(linesYaml[a].contains("name"))
        {
            numPlasmid = numPlasmid + 1;
        }
        a = a + 1;
    }
    qDebug() << "sale del bucle de contar plasmidos "+linesYaml[numeroLineaDeYaml];
    QString nombrePlasmid[numPlasmid];
    QString operonPlasmid[numPlasmid][nombreElementGenetics.size()];
    for(int i = 0; i < numPlasmid; i++)
    {
        nombrePlasmid[i] = "";
        for(int j = 0; j < nombreElementGenetics.size(); j++)
        {
            operonPlasmid[i][j] = "";
        }
    }
    qDebug() << "sale de los dos bucles "+linesYaml[numeroLineaDeYaml];
    a = 0;
    QString prueba = "";
    QString operonDePlasmidoI[nombreElementGenetics.size()+1];
    for(int i = 0; i < nombreElementGenetics.size()+1; i++)
    {
        operonDePlasmidoI[i] = "";
    }
    //traducimos la parte de plasmidos
    if(linesYaml[numeroLineaDeYaml].contains("plasmid"))
    {
        qDebug() << "entra en plasmido "+linesYaml[numeroLineaDeYaml];
        int b = numeroLineaDeYaml;
        int contPlasmid = 0;
        while(linesYaml[b].contains("simulations") == false && linesYaml[b].contains("signals") == false && linesYaml[b].contains("strains") == false && linesYaml[b].contains("cell") == false && linesYaml[b].contains("world") == false && linesYaml[b].contains("output") == false)
        {
           b++;
           if(linesYaml[b].contains("name"))
           {
               contPlasmid++;
           }
        }
        a = 0;
        qDebug() << "sale de contPlasmid ";
        while(linesYaml[numeroLineaDeYaml].contains("simulations") == false && linesYaml[numeroLineaDeYaml].contains("signals") == false && linesYaml[numeroLineaDeYaml].contains("strains") == false && linesYaml[numeroLineaDeYaml].contains("cell") == false && linesYaml[numeroLineaDeYaml].contains("world") == false && linesYaml[numeroLineaDeYaml].contains("output") == false)
        {
              numeroLineaDeYaml++;
              if(linesYaml[numeroLineaDeYaml].contains("name"))
              {
                  qDebug() << "entra nameplasmid "+linesYaml[numeroLineaDeYaml];
                  nombrePlasmid[a] = linesYaml[numeroLineaDeYaml].replace("-name:","");
              }
              if(linesYaml[numeroLineaDeYaml].contains("operons"))
              {
                  qDebug() << "entra en operonesplasmid "+linesYaml[numeroLineaDeYaml];
                  linesYaml[numeroLineaDeYaml].replace("operons:[","").replace("]","");
                  aux.clear();
                  aux = linesYaml[numeroLineaDeYaml].split(",");
                  for(int k = 0; k < aux.size(); k++)
                  {       
                      for(int m = 0; m < nombreElementGenetics.size(); m++)
                      {
                          if(aux[k] == nombreOperon[m])
                          {
                              contadore = 1;
                          }
                          qDebug() << "aux[i] "+aux[k];
                          qDebug() << "nombre Operon [j] "+nombreOperon[m];
                          qDebug() << contadore;
                      }
                      if(contadore == 0)
                      {
                          numeroString = QString::number(numeroLineaDeYaml+1);
                          textoSemanticoCorrecto = false;
                          errorSemantico.append("\nERROR: There are operons in plasmid that they aren't defined in operons.\n");
                      }
                      contadore = 0;
                  }

                  if(textoSemanticoCorrecto)
                  {
                      for(int i = 0; i < aux.size(); i++)
                      {
                          qDebug() << "aux[i]";
                          qDebug() << aux[i];
                          qDebug() << "a";
                          qDebug() << a;
                          qDebug() << "i";
                          qDebug() << i;
                          qDebug() << "contPlasmid";
                          qDebug() << contPlasmid;
                              if(aux[i].isEmpty() == false || aux[i].isNull() == false || aux[i] == "")
                              {
                                  operonPlasmid[a][i] = aux[i];
                                  qDebug() << "entro en operon=aux"+operonPlasmid[a][i];
                              }

                              qDebug() << "operonPlasmid"+operonPlasmid[a][i];
                      }
                  }

                  a++;
              }

              qDebug() << "pasa el for 543 "+linesYaml[numeroLineaDeYaml];
              qDebug() << "hace bien prueba "+prueba;
        }
        qDebug() << "linea535 "+linesYaml[numeroLineaDeYaml];

        for(int i = 0; i < numPlasmid; i++)
        {
            qDebug() << "valor i ";
            qDebug() << i;
            for(int j = 0; j < nombreElementGenetics.size(); j++)
            {
                qDebug() << "valor j";
                qDebug() << j;
                qDebug() << "operon plasmido aj "+operonPlasmid[i][j];
                if(operonPlasmid[i][j] != "" || operonPlasmid[i][j].isEmpty() == false || operonPlasmid[i][j].isNull() == false)
                {
                    qDebug() << "entra en el if";

                    operonDePlasmidoI[i] = operonDePlasmidoI[i]+ "\""+operonPlasmid[i][j]+"\",";
                    qDebug() << operonDePlasmidoI[i];
                }

                qDebug() << "sale del if";
            }
            prueba = prueba +" "+nombrePlasmid[i]+" := {"+operonDePlasmidoI[i]+"}, ";
            qDebug() << prueba;
        }

        qDebug() << "sale del while linea 549"+linesYaml[numeroLineaDeYaml];
    }
    prueba.replace(",\"\"","");
    qDebug() << prueba+ "esto es prueba ";
    QString auxiliarPlasmidos = "plasmids_genes ([ "+prueba+"]);";
    auxiliarPlasmidos = auxiliarPlasmidos.replace(",}","}").replace("}, ]","}]");

    linesGro.append("\n"+auxiliarPlasmidos+"\n");
    return numeroLineaDeYaml;
}
//metodo que se encarga de traducir la parte de strains
//este metodo graba en variables los atributos de strains, no traduce nada hasta
//llegar a world actions
int VentanaPrincipal::metodoStrain(int lineaActual)
{
    contCepas = 0;
    numeroLineaDeYaml = lineaActual;
    while(linesYaml[lineaActual].contains("cell_actions") == false && linesYaml[lineaActual].contains("world_actions") == false && linesYaml[lineaActual].contains("output") == false)
    {
        if(linesYaml[lineaActual].contains("name"))
        {
            contCepas++;
        }
        lineaActual++;
    }
    QString nombreCepas[contCepas];
    QString anchuraCepas[contCepas];
    QString radCrecCepas[contCepas];
    QString longDivCepas[contCepas][2];
    QStringList lDivCepas;
    QString divProcCepas[contCepas][2];
    QStringList dProcCepas;
    QString plasCepas[contCepas];
    for(int i = 0; i < contCepas; i++)
    {
        nombreCepas[i] = "";
        anchuraCepas[i] = "";
        radCrecCepas[i] = "";
        longDivCepas[i][0] = "";
        longDivCepas[i][1] = "";
        divProcCepas[i][0] = "";
        divProcCepas[i][1] = "";
        plasCepas[i] = "";
    }
    int j = 0;
    while(linesYaml[numeroLineaDeYaml].contains("cell_actions") == false && linesYaml[numeroLineaDeYaml].contains("world_actions") == false && linesYaml[numeroLineaDeYaml].contains("output") == false)
    {
        if(linesYaml[numeroLineaDeYaml].contains("name"))
        {
            nombreCepas[j] = linesYaml[numeroLineaDeYaml].replace("-name:","");
            nombreGlobalCepa.append(nombreCepas[j]);
            qDebug() <<"nombre cepa j "+nombreCepas[j];
            j++;
        }
        if(linesYaml[numeroLineaDeYaml].contains("width"))
        {
            j--;
            anchuraCepas[j] = linesYaml[numeroLineaDeYaml].replace("width:","");
            if(validaNumeroFloat(anchuraCepas[j]) == false)
            {
                numeroString = QString::number(numeroLineaDeYaml+1);
                textoSemanticoCorrecto = false;
                errorSemantico.append("\nERROR line "+numeroString+": the width of strain has to be a number (float)\n");
            }
            j++;
        }
        if(linesYaml[numeroLineaDeYaml].contains("default_growth"))
        {
            j--;
            radCrecCepas[j] = linesYaml[numeroLineaDeYaml].replace("default_growth_rate:","");
            if(validaNumeroFloat(radCrecCepas[j]) == false)
            {
                numeroString = QString::number(numeroLineaDeYaml+1);
                textoSemanticoCorrecto = false;
                errorSemantico.append("\nERROR line "+numeroString+": the default_growth_rate of strain has to be a number (float)\n");
            }
            qDebug() <<"radCrec cepa j "+radCrecCepas[j];
            j++;
        }
        if(linesYaml[numeroLineaDeYaml].contains("division_length"))
        {
            j--;
            linesYaml[numeroLineaDeYaml].replace("division_length:{x:","").replace("y:","").replace("}","");
            lDivCepas.clear();
            lDivCepas = linesYaml[numeroLineaDeYaml].split(",");
            longDivCepas[j][0] = lDivCepas[0];
            longDivCepas[j][1] = lDivCepas[1];
            if(validaNumeroFloat(longDivCepas[j][0]) == false || validaNumeroFloat(longDivCepas[j][1]) == false)
            {
                numeroString = QString::number(numeroLineaDeYaml+1);
                textoSemanticoCorrecto = false;
                errorSemantico.append("\nERROR line "+numeroString+": the division_length of strain has to be a number (float)\n");
            }
            j++;
        }
        if(linesYaml[numeroLineaDeYaml].contains("division_proportion"))
        {
            j--;
            linesYaml[numeroLineaDeYaml].replace("division_proportion:{x:","").replace("y:","").replace("}","");
            dProcCepas.clear();;
            dProcCepas = linesYaml[numeroLineaDeYaml].split(",");
            divProcCepas[j][0] = dProcCepas[0];
            divProcCepas[j][1] = dProcCepas[1];
            if(validaNumeroFloat(divProcCepas[j][0]) == false || validaNumeroFloat(divProcCepas[j][1]) == false)
            {
                numeroString = QString::number(numeroLineaDeYaml+1);
                textoSemanticoCorrecto = false;
                errorSemantico.append("\nERROR line "+numeroString+": the division_proportion of strain has to be a number (float)\n");
            }
            j++;
        }
        if(linesYaml[numeroLineaDeYaml].contains("plasmids"))
        {
            j--;
            linesYaml[numeroLineaDeYaml].replace("plasmids:","").replace("[","").replace("]","");
            plasCepas[j] = linesYaml[numeroLineaDeYaml];
            j++;
        }
        numeroLineaDeYaml++;
    }
    if(textoSemanticoCorrecto)
    {
        for(int i = 0; i < contCepas; i++)
        {
            nomCepas.append(nombreCepas[i]);
            qDebug() << "variable global nomCepas "+nomCepas[i];
            plCepas.append(plasCepas[i]);
            radCrecCep.append(radCrecCepas[i]);
            qDebug() << "variable global radCrecCepas "+radCrecCep[i];
        }
    }


    return numeroLineaDeYaml;
}
//metodo que se encarga de traducir la parte de cell actions
int VentanaPrincipal::metodoCellActions(int lineaActual)
{
    //metodo que traduce las acciones a una linea, es muy simple pero tiene muchas acciones diferentes
    numeroLineaDeYaml = lineaActual;
    QString cellAction = "";
    QStringList cellActionAux;
    QStringList coloraction;
    bool masDeUno = false;
    while(linesYaml[numeroLineaDeYaml].contains("world_act") == false && linesYaml[numeroLineaDeYaml].contains("output") == false)
    {
        qDebug() << "714 "+linesYaml[numeroLineaDeYaml];
        //paint
        if(linesYaml[numeroLineaDeYaml].contains("paint") && linesYaml[numeroLineaDeYaml].contains("d_pa") == false)
        {
            numeroLineaDeYaml++;
            while(linesYaml[numeroLineaDeYaml].contains("world_act") == false && linesYaml[numeroLineaDeYaml].contains("output") == false && linesYaml[numeroLineaDeYaml].contains("paint") == false && linesYaml[numeroLineaDeYaml].contains("die") == false && linesYaml[numeroLineaDeYaml].contains("conjugation") == false && linesYaml[numeroLineaDeYaml].contains("lose") == false && linesYaml[numeroLineaDeYaml].contains("entry_exclusion") == false && linesYaml[numeroLineaDeYaml].contains("groth_rate") == false && linesYaml[numeroLineaDeYaml].contains("emit") == false && linesYaml[numeroLineaDeYaml].contains("quorum") == false && linesYaml[numeroLineaDeYaml].contains("feeding") == false)
            {
                if(linesYaml[numeroLineaDeYaml].contains("condition"))
                {
                    if(linesYaml[numeroLineaDeYaml].contains("[]"))
                    {
                        cellActionAux.append("");
                    }
                    else
                    {
                        linesYaml[numeroLineaDeYaml].replace("condition:[","").replace("]","");
                        if(linesYaml[numeroLineaDeYaml].contains(","))
                        {
                            cellActionAux = linesYaml[numeroLineaDeYaml].split(",");
                            masDeUno = true;
                        }
                        else
                        {
                            cellAction = linesYaml[numeroLineaDeYaml];
                        }
                    }
                }
                if(linesYaml[numeroLineaDeYaml].contains("color"))
                {
                    coloraction = linesYaml[numeroLineaDeYaml].replace("color:[","").replace("]","").split(",");
                }
                numeroLineaDeYaml++;
            }
            QString colores = "";
            for(int i = 0; i < coloraction.size(); i++)
            {
                colores = colores + "\""+coloraction[i]+"\",";
            }
            colores = "{"+colores+"}";
            colores.replace(",}","}");
            if(masDeUno)
            {
                QString celulasAPaint = "";
                for(int i = 0; i < cellActionAux.size(); i++)
                {
                    celulasAPaint = celulasAPaint+"\""+cellActionAux[i]+"\",";
                }
                celulasAPaint = "{"+celulasAPaint+"}";
                celulasAPaint.replace(",}","}");
                linesGro.append("\naction("+celulasAPaint+", \"paint\" , "+colores+");\n");
            }
            else
            {
                linesGro.append("\naction("+cellAction+", \"paint\" , "+colores+");\n");
            }
        }

        qDebug() << "771 "+linesYaml[numeroLineaDeYaml];
        //d_paint
        cellAction = "";
        cellActionAux.clear();
        coloraction.clear();
        masDeUno = false;
        if(linesYaml[numeroLineaDeYaml].contains("d_paint"))
        {
            numeroLineaDeYaml++;
            while(linesYaml[numeroLineaDeYaml].contains("world_act") == false && linesYaml[numeroLineaDeYaml].contains("output") == false && linesYaml[numeroLineaDeYaml].contains("paint") == false && linesYaml[numeroLineaDeYaml].contains("die") == false && linesYaml[numeroLineaDeYaml].contains("conjugation") == false && linesYaml[numeroLineaDeYaml].contains("lose") == false && linesYaml[numeroLineaDeYaml].contains("entry_exclusion") == false && linesYaml[numeroLineaDeYaml].contains("groth_rate") == false && linesYaml[numeroLineaDeYaml].contains("emit") == false && linesYaml[numeroLineaDeYaml].contains("quorum") == false && linesYaml[numeroLineaDeYaml].contains("feeding") == false)
            {
                if(linesYaml[numeroLineaDeYaml].contains("condition"))
                {
                    if(linesYaml[numeroLineaDeYaml].contains("[]"))
                    {
                        cellActionAux.append("");
                    }
                    else
                    {
                        linesYaml[numeroLineaDeYaml].replace("condition:[","").replace("]","");
                        if(linesYaml[numeroLineaDeYaml].contains(","))
                        {
                            cellActionAux = linesYaml[numeroLineaDeYaml].split(",");
                            masDeUno = true;
                        }
                        else
                        {
                            cellAction = linesYaml[numeroLineaDeYaml];
                        }
                    }
                }
                if(linesYaml[numeroLineaDeYaml].contains("color"))
                {
                    coloraction = linesYaml[numeroLineaDeYaml].replace("color:[","").replace("]","").split(",");
                }
                numeroLineaDeYaml++;
            }
            QString colores = "";
            for(int i = 0; i < coloraction.size(); i++)
            {
                colores = colores + "\""+coloraction[i]+"\",";
            }
            colores = "{"+colores+"}";
            colores.replace(",}","}");
            if(masDeUno)
            {
                QString celulasAPaint = "";
                for(int i = 0; i < cellActionAux.size(); i++)
                {
                    celulasAPaint = celulasAPaint+"\""+cellActionAux[i]+"\",";
                }
                celulasAPaint = "{"+celulasAPaint+"}";
                celulasAPaint.replace(",}","}");
                linesGro.append("\naction("+celulasAPaint+", \"d_paint\" , "+colores+");\n");
            }
            else
            {
                linesGro.append("\naction({"+cellAction+"}, \"d_paint\" , "+colores+");\n");
            }
        }

        qDebug() << "831 "+linesYaml[numeroLineaDeYaml];
        //die
        cellAction = "";
        cellActionAux.clear();
        masDeUno = false;
        if(linesYaml[numeroLineaDeYaml].contains("die"))
        {
            numeroLineaDeYaml++;
            while(linesYaml[numeroLineaDeYaml].contains("world_act") == false && linesYaml[numeroLineaDeYaml].contains("output") == false && linesYaml[numeroLineaDeYaml].contains("paint") == false && linesYaml[numeroLineaDeYaml].contains("die") == false && linesYaml[numeroLineaDeYaml].contains("conjugation") == false && linesYaml[numeroLineaDeYaml].contains("lose") == false && linesYaml[numeroLineaDeYaml].contains("entry_exclusion") == false && linesYaml[numeroLineaDeYaml].contains("groth_rate") == false && linesYaml[numeroLineaDeYaml].contains("emit") == false && linesYaml[numeroLineaDeYaml].contains("quorum") == false && linesYaml[numeroLineaDeYaml].contains("feeding") == false)
            {
                if(linesYaml[numeroLineaDeYaml].contains("condition"))
                {
                    if(linesYaml[numeroLineaDeYaml].contains("[]"))
                    {
                        cellActionAux.append("");
                    }
                    else
                    {
                        if(linesYaml[numeroLineaDeYaml].contains(",") == false)
                        {
                            cellAction = linesYaml[numeroLineaDeYaml].replace("condition:[","").replace("]","");
                        }
                        else
                        {
                            masDeUno = true;
                            cellActionAux = linesYaml[numeroLineaDeYaml].replace("condition:","").replace("[","").replace("]","").split(",");
                        }
                    }
                }
                numeroLineaDeYaml++;
            }
            if(masDeUno)
            {
                QString celulasAMorir = "";
                for(int i = 0; i < cellActionAux.size(); i++)
                {
                    celulasAMorir = celulasAMorir + "\""+cellActionAux[i]+"\",";
                }
                celulasAMorir = "{"+celulasAMorir+"}";
                celulasAMorir = celulasAMorir.replace(",}","}");
                linesGro.append("\naction("+celulasAMorir+", \"die\" , {\"0\"});\n");
            }
            else
            {
                linesGro.append("\naction({"+cellAction+"}, \"die\" , {\"0\"});\n");
            }
        }

        qDebug() << "878 "+linesYaml[numeroLineaDeYaml];
        //conjugate
        masDeUno = false;
        cellAction = "";
        cellActionAux.clear();
        if(linesYaml[numeroLineaDeYaml].contains("-conjugation"))
        {
            QString plasmidConjugate = "";
            QString plasmidRate = "";
            numeroLineaDeYaml++;
            while(linesYaml[numeroLineaDeYaml].contains("world_act") == false && linesYaml[numeroLineaDeYaml].contains("output") == false && linesYaml[numeroLineaDeYaml].contains("paint") == false && linesYaml[numeroLineaDeYaml].contains("die") == false && linesYaml[numeroLineaDeYaml].contains("conjugation") == false && linesYaml[numeroLineaDeYaml].contains("lose") == false && linesYaml[numeroLineaDeYaml].contains("entry_exclusion") == false && linesYaml[numeroLineaDeYaml].contains("groth_rate") == false && linesYaml[numeroLineaDeYaml].contains("emit") == false && linesYaml[numeroLineaDeYaml].contains("quorum") == false && linesYaml[numeroLineaDeYaml].contains("feeding") == false)
            {
                if(linesYaml[numeroLineaDeYaml].contains("condition"))
                {
                    if(linesYaml[numeroLineaDeYaml].contains("[]"))
                    {
                        cellActionAux.append("");
                    }
                    else
                    {
                        if(linesYaml[numeroLineaDeYaml].contains(","))
                        {
                            cellActionAux = linesYaml[numeroLineaDeYaml].replace("condition:[","").replace("]","").split(",");
                            masDeUno = true;
                        }
                        else
                        {
                            cellAction = linesYaml[numeroLineaDeYaml].replace("condition:[","").replace("]","");
                        }
                    }
                }
                if(linesYaml[numeroLineaDeYaml].contains("plasmid"))
                {
                    plasmidConjugate = linesYaml[numeroLineaDeYaml].replace("plasmid:","");
                }
                if(linesYaml[numeroLineaDeYaml].contains("rate"))
                {
                    plasmidRate = linesYaml[numeroLineaDeYaml].replace("rate:","");
                }
                numeroLineaDeYaml++;
            }
            if(masDeUno)
            {
                QString celulasAConjugate = "";
                for(int i = 0; i < cellActionAux.size(); i++)
                {
                    celulasAConjugate = celulasAConjugate + "\""+cellActionAux[i]+"\",";
                }
                celulasAConjugate = "{"+celulasAConjugate+"}";
                celulasAConjugate = celulasAConjugate.replace(",}","}");
                linesGro.append("\naction("+celulasAConjugate+", \"conjugation\" , {\""+plasmidConjugate+"\",\""+plasmidRate+"\"});\n");
            }
            else
            {
                linesGro.append("\naction({"+cellAction+"}, \"conjugation\" , {\""+plasmidConjugate+"\",\""+plasmidRate+"\"});\n");
            }

        }

        qDebug() << "939 "+linesYaml[numeroLineaDeYaml];
        //directed_conjugate
        cellActionAux.clear();
        masDeUno = false;
        cellAction = "";
        if(linesYaml[numeroLineaDeYaml].contains("-directed_conjugation"))
        {
            QString plasmidConjugate = "";
            QString plasmidRate = "";
            numeroLineaDeYaml++;
            while(linesYaml[numeroLineaDeYaml].contains("world_act") == false && linesYaml[numeroLineaDeYaml].contains("output") == false && linesYaml[numeroLineaDeYaml].contains("paint") == false && linesYaml[numeroLineaDeYaml].contains("die") == false && linesYaml[numeroLineaDeYaml].contains("conjugation") == false && linesYaml[numeroLineaDeYaml].contains("lose") == false && linesYaml[numeroLineaDeYaml].contains("entry_exclusion") == false && linesYaml[numeroLineaDeYaml].contains("groth_rate") == false && linesYaml[numeroLineaDeYaml].contains("emit") == false && linesYaml[numeroLineaDeYaml].contains("quorum") == false && linesYaml[numeroLineaDeYaml].contains("feeding") == false)
            {
                if(linesYaml[numeroLineaDeYaml].contains("condition"))
                {
                    if(linesYaml[numeroLineaDeYaml].contains("[]"))
                    {
                        cellActionAux.append("");
                    }
                    else
                    {
                        if(linesYaml[numeroLineaDeYaml].contains(","))
                        {
                            masDeUno = true;
                            cellActionAux = linesYaml[numeroLineaDeYaml].replace("condition:[","").replace("]","").split(",");
                        }
                        else
                        {
                            cellAction = linesYaml[numeroLineaDeYaml].replace("condition:[","").replace("]","");
                        }
                    }
                }
                if(linesYaml[numeroLineaDeYaml].contains("plasmid"))
                {
                    plasmidConjugate = linesYaml[numeroLineaDeYaml].replace("plasmid:","");
                }
                if(linesYaml[numeroLineaDeYaml].contains("rate"))
                {
                    plasmidRate = linesYaml[numeroLineaDeYaml].replace("rate:","");
                }
                numeroLineaDeYaml++;
            }
            if(masDeUno)
            {
                QString celulasAConjugate = "";
                for(int i = 0; i < cellActionAux.size(); i++)
                {
                    celulasAConjugate = celulasAConjugate + "\""+cellActionAux[i]+"\",";
                }
                celulasAConjugate = "{"+celulasAConjugate+"}";
                celulasAConjugate = celulasAConjugate.replace(",}","}");
                linesGro.append("\naction("+celulasAConjugate+", \"directed_conjugation\" , {\""+plasmidConjugate+"\",\""+plasmidRate+"\"});\n");

            }
            else
            {
                linesGro.append("\naction({"+cellAction+"}, \"directed_conjugation\" , {\""+plasmidConjugate+"\",\""+plasmidRate+"\"});\n");
            }
        }

        qDebug() << "998 "+linesYaml[numeroLineaDeYaml];
        //lose_plasmid
        cellActionAux.clear();
        masDeUno = false;
        cellAction = "";
        if(linesYaml[numeroLineaDeYaml].contains("lose_plasmid"))
        {
            QString plasmidLose = "";
            numeroLineaDeYaml++;
            while(linesYaml[numeroLineaDeYaml].contains("world_act") == false && linesYaml[numeroLineaDeYaml].contains("output") == false && linesYaml[numeroLineaDeYaml].contains("paint") == false && linesYaml[numeroLineaDeYaml].contains("die") == false && linesYaml[numeroLineaDeYaml].contains("conjugation") == false && linesYaml[numeroLineaDeYaml].contains("lose") == false && linesYaml[numeroLineaDeYaml].contains("entry_exclusion") == false && linesYaml[numeroLineaDeYaml].contains("groth_rate") == false && linesYaml[numeroLineaDeYaml].contains("emit") == false && linesYaml[numeroLineaDeYaml].contains("quorum") == false && linesYaml[numeroLineaDeYaml].contains("feeding") == false)
            {
                if(linesYaml[numeroLineaDeYaml].contains("condition"))
                {
                    if(linesYaml[numeroLineaDeYaml].contains("[]"))
                    {
                        cellActionAux.append("");
                    }
                    else
                    {
                        if(linesYaml[numeroLineaDeYaml].contains(","))
                        {
                            cellActionAux = linesYaml[numeroLineaDeYaml].replace("condition:[","").replace("]","").split(",");
                            masDeUno = true;
                        }
                        else
                        {
                            cellAction = linesYaml[numeroLineaDeYaml].replace("condition:[","").replace("]","");
                        }
                    }
                }
                if(linesYaml[numeroLineaDeYaml].contains("plasmid"))
                {
                    plasmidLose = linesYaml[numeroLineaDeYaml].replace("plasmid:","");
                }
                numeroLineaDeYaml++;
            }
            if(masDeUno)
            {
                QString celulasALose = "";
                for(int i = 0; i < cellActionAux.size(); i++)
                {
                    celulasALose = celulasALose + "\""+cellActionAux[i]+"\",";
                }
                celulasALose = "{"+celulasALose+"}";
                celulasALose.replace(",}","}");
                linesGro.append("\naction("+celulasALose+", \"lose_plasmid\" , {\""+plasmidLose+"\"});\n");
            }
            else
            {
                linesGro.append("\naction({"+cellAction+"}, \"lose_plasmid\" , {\""+plasmidLose+"\"});\n");
            }
        }
        //set_entry_exclusion
        cellAction = "";
        masDeUno = false;
        cellActionAux.clear();
        if(linesYaml[numeroLineaDeYaml].contains("set_entry_exclusion"))
        {
            QString plasmidSee = "";
            numeroLineaDeYaml++;
            while(linesYaml[numeroLineaDeYaml].contains("world_act") == false && linesYaml[numeroLineaDeYaml].contains("output") == false && linesYaml[numeroLineaDeYaml].contains("paint") == false && linesYaml[numeroLineaDeYaml].contains("die") == false && linesYaml[numeroLineaDeYaml].contains("conjugation") == false && linesYaml[numeroLineaDeYaml].contains("lose") == false && linesYaml[numeroLineaDeYaml].contains("entry_exclusion") == false && linesYaml[numeroLineaDeYaml].contains("groth_rate") == false && linesYaml[numeroLineaDeYaml].contains("emit") == false && linesYaml[numeroLineaDeYaml].contains("quorum") == false && linesYaml[numeroLineaDeYaml].contains("feeding") == false)
            {
                if(linesYaml[numeroLineaDeYaml].contains("condition"))
                {
                    if(linesYaml[numeroLineaDeYaml].contains("[]"))
                    {
                        cellActionAux.append("");
                    }
                    else
                    {
                        if(linesYaml[numeroLineaDeYaml].contains(","))
                        {
                            cellActionAux = linesYaml[numeroLineaDeYaml].replace("condition:[","").replace("]","").split(",");
                            masDeUno = true;
                        }
                        else
                        {
                            cellAction = linesYaml[numeroLineaDeYaml].replace("condition:[","").replace("]","");
                        }
                    }
                }
                if(linesYaml[numeroLineaDeYaml].contains("plasmid"))
                {
                    plasmidSee = linesYaml[numeroLineaDeYaml].replace("plasmid:","");
                }
                numeroLineaDeYaml++;
            }
            if(masDeUno)
            {
                QString celulasASee = "";
                for(int i = 0; i < cellActionAux.size(); i++)
                {
                    celulasASee = celulasASee + "\""+cellActionAux[i]+"\",";
                }
                celulasASee = "{"+celulasASee+"}";
                celulasASee.replace(",}","}");
                linesGro.append("\naction("+celulasASee+", \"set_entry_exclusion\" , {\""+plasmidSee+"\"});\n");
            }
            else
            {
                linesGro.append("\naction({"+cellAction+"}, \"set_entry_exclusion\" , {\""+plasmidSee+"\"});\n");
            }
        }
        //remove_entry_exclusion
        cellAction = "";
        masDeUno = false;
        cellActionAux.clear();
        if(linesYaml[numeroLineaDeYaml].contains("remove_entry_exclusion"))
        {
            QString plasmidRee = "";
            numeroLineaDeYaml++;
            while(linesYaml[numeroLineaDeYaml].contains("world_act") == false && linesYaml[numeroLineaDeYaml].contains("output") == false && linesYaml[numeroLineaDeYaml].contains("paint") == false && linesYaml[numeroLineaDeYaml].contains("die") == false && linesYaml[numeroLineaDeYaml].contains("conjugation") == false && linesYaml[numeroLineaDeYaml].contains("lose") == false && linesYaml[numeroLineaDeYaml].contains("entry_exclusion") == false && linesYaml[numeroLineaDeYaml].contains("groth_rate") == false && linesYaml[numeroLineaDeYaml].contains("emit") == false && linesYaml[numeroLineaDeYaml].contains("quorum") == false && linesYaml[numeroLineaDeYaml].contains("feeding") == false)
            {
                if(linesYaml[numeroLineaDeYaml].contains("condition"))
                {
                    if(linesYaml[numeroLineaDeYaml].contains("[]"))
                    {
                        cellActionAux.append("");
                    }
                    else
                    {
                        if(linesYaml[numeroLineaDeYaml].contains(","))
                        {
                            cellActionAux = linesYaml[numeroLineaDeYaml].replace("condition:[","").replace("]","").split(",");
                            masDeUno = true;
                        }
                        else
                            cellAction = linesYaml[numeroLineaDeYaml].replace("condition:[","").replace("]","");
                    }
                }
                if(linesYaml[numeroLineaDeYaml].contains("plasmid"))
                {
                    plasmidRee = linesYaml[numeroLineaDeYaml].replace("plasmid:","");
                }
                numeroLineaDeYaml++;
            }
            if(masDeUno)
            {
                QString celulasARee = "";
                for(int i = 0; i < cellActionAux.size(); i++)
                {
                    celulasARee = celulasARee + "\""+cellActionAux[i]+"\",";
                }
                celulasARee = "{"+celulasARee+"}";
                celulasARee.replace(",}","}");
                linesGro.append("\naction("+celulasARee+", \"remove_entry_exclusion\" , {\""+plasmidRee+"\"});\n");
            }
            else
            {
                linesGro.append("\naction({"+cellAction+"}, \"remove_entry_exclusion\" , {\""+plasmidRee+"\"});\n");
            }
        }
        //set_growth_rate
        cellAction = "";
        masDeUno = false;
        cellActionAux.clear();
        if(linesYaml[numeroLineaDeYaml].contains("set_growth_rate"))
        {
            QString plasmidSgr = "";
            numeroLineaDeYaml++;
            while(linesYaml[numeroLineaDeYaml].contains("world_act") == false && linesYaml[numeroLineaDeYaml].contains("output") == false && linesYaml[numeroLineaDeYaml].contains("paint") == false && linesYaml[numeroLineaDeYaml].contains("die") == false && linesYaml[numeroLineaDeYaml].contains("conjugation") == false && linesYaml[numeroLineaDeYaml].contains("lose") == false && linesYaml[numeroLineaDeYaml].contains("entry_exclusion") == false && linesYaml[numeroLineaDeYaml].contains("groth_rate") == false && linesYaml[numeroLineaDeYaml].contains("emit") == false && linesYaml[numeroLineaDeYaml].contains("quorum") == false && linesYaml[numeroLineaDeYaml].contains("feeding") == false)
            {
                if(linesYaml[numeroLineaDeYaml].contains("condition"))
                {
                    if(linesYaml[numeroLineaDeYaml].contains("[]"))
                    {
                        cellActionAux.append("");
                    }
                    else
                    {
                        if(linesYaml[numeroLineaDeYaml].contains(","))
                        {
                            cellActionAux = linesYaml[numeroLineaDeYaml].replace("condition:[","").replace("]","").split(",");
                            masDeUno = true;
                        }
                        else
                        {
                            cellAction = linesYaml[numeroLineaDeYaml].replace("condition:[","").replace("]","");
                        }
                    }
                }
                if(linesYaml[numeroLineaDeYaml].contains("rate"))
                {
                    plasmidSgr = linesYaml[numeroLineaDeYaml].replace("rate:","");
                }
                numeroLineaDeYaml++;
            }
            if(masDeUno)
            {
                QString celulasASgr = "";
                for(int i = 0; i < cellActionAux.size(); i++)
                {
                    celulasASgr = celulasASgr + "\""+cellActionAux[i]+"\",";
                }
                celulasASgr = "{"+celulasASgr+"}";
                celulasASgr.replace(",}","}");
                linesGro.append("\naction("+celulasASgr+", \"set_growth_rate\" , {\""+plasmidSgr+"\"});\n");
            }
            else
            {
                linesGro.append("\naction({"+cellAction+"}, \"set_growth_rate\" , {\""+plasmidSgr+"\"});\n");
            }

        }
        //emit
        cellActionAux.clear();
        cellAction = "";
        masDeUno = false;
        if(linesYaml[numeroLineaDeYaml].contains("emit:"))
        {
            QString concentrationEmit = "";
            QString signalEit = "";
            numeroLineaDeYaml++;
            while(linesYaml[numeroLineaDeYaml].contains("world_act") == false && linesYaml[numeroLineaDeYaml].contains("output") == false && linesYaml[numeroLineaDeYaml].contains("paint") == false && linesYaml[numeroLineaDeYaml].contains("die") == false && linesYaml[numeroLineaDeYaml].contains("conjugation") == false && linesYaml[numeroLineaDeYaml].contains("lose") == false && linesYaml[numeroLineaDeYaml].contains("entry_exclusion") == false && linesYaml[numeroLineaDeYaml].contains("groth_rate") == false && linesYaml[numeroLineaDeYaml].contains("emit") == false && linesYaml[numeroLineaDeYaml].contains("quorum") == false && linesYaml[numeroLineaDeYaml].contains("feeding") == false)
            {
                if(linesYaml[numeroLineaDeYaml].contains("condition"))
                {
                    if(linesYaml[numeroLineaDeYaml].contains("[]"))
                    {
                        cellActionAux.append("");
                    }
                    else
                    {
                        if(linesYaml[numeroLineaDeYaml].contains(","))
                        {
                            cellActionAux = linesYaml[numeroLineaDeYaml].replace("condition:[","").replace("]","").split(",");
                            masDeUno = true;
                        }
                        else
                        {
                            cellAction = linesYaml[numeroLineaDeYaml].replace("condition:[","").replace("]","");
                        }
                    }
                }
                if(linesYaml[numeroLineaDeYaml].contains("signal"))
                {
                    signalEit = linesYaml[numeroLineaDeYaml].replace("signal:","");
                }
                if(linesYaml[numeroLineaDeYaml].contains("concentration"))
                {
                    concentrationEmit = linesYaml[numeroLineaDeYaml].replace("concentration:","");
                }
                numeroLineaDeYaml++;
            }
            if(masDeUno)
            {
                QString celulasASgr = "";
                for(int i = 0; i < cellActionAux.size(); i++)
                {
                    celulasASgr = celulasASgr + "\""+cellActionAux[i]+"\",";
                }
                celulasASgr = "{"+celulasASgr+"}";
                celulasASgr.replace(",}","}");
                linesGro.append("\naction("+celulasASgr+", \"emit\" , {tostring("+signalEit+"),\""+concentrationEmit+"\"});\n");

            }
            else
            {
                linesGro.append("\naction({"+cellAction+"}, \"emit\" , {tostring("+signalEit+"),\""+concentrationEmit+"\"});\n");
            }
        }
        //emit_area
        cellActionAux.clear();
        cellAction = "";
        masDeUno = false;
        if(linesYaml[numeroLineaDeYaml].contains("emit_area:"))
        {
            QString concentrationEmit = "";
            QString signalEit = "";
            numeroLineaDeYaml++;
            while(linesYaml[numeroLineaDeYaml].contains("world_act") == false && linesYaml[numeroLineaDeYaml].contains("output") == false && linesYaml[numeroLineaDeYaml].contains("paint") == false && linesYaml[numeroLineaDeYaml].contains("die") == false && linesYaml[numeroLineaDeYaml].contains("conjugation") == false && linesYaml[numeroLineaDeYaml].contains("lose") == false && linesYaml[numeroLineaDeYaml].contains("entry_exclusion") == false && linesYaml[numeroLineaDeYaml].contains("groth_rate") == false && linesYaml[numeroLineaDeYaml].contains("emit") == false && linesYaml[numeroLineaDeYaml].contains("quorum") == false && linesYaml[numeroLineaDeYaml].contains("feeding") == false)
            {
                if(linesYaml[numeroLineaDeYaml].contains("condition"))
                {
                    if(linesYaml[numeroLineaDeYaml].contains("[]"))
                    {
                        cellActionAux.append("");
                    }
                    else
                    {
                        if(linesYaml[numeroLineaDeYaml].contains(","))
                        {
                            cellActionAux = linesYaml[numeroLineaDeYaml].replace("condition:[","").replace("]","").split(",");
                            masDeUno = true;
                        }
                        else
                        {
                            cellAction = linesYaml[numeroLineaDeYaml].replace("condition:[","").replace("]","");
                        }
                    }
                }
                if(linesYaml[numeroLineaDeYaml].contains("signal"))
                {
                    signalEit = linesYaml[numeroLineaDeYaml].replace("signal:","");
                }
                if(linesYaml[numeroLineaDeYaml].contains("concentration"))
                {
                    concentrationEmit = linesYaml[numeroLineaDeYaml].replace("concentration:","");
                }
                numeroLineaDeYaml++;
            }
            if(masDeUno)
            {
                QString celulasASgr = "";
                for(int i = 0; i < cellActionAux.size(); i++)
                {
                    celulasASgr = celulasASgr + "\""+cellActionAux[i]+"\",";
                }
                celulasASgr = "{"+celulasASgr+"}";
                celulasASgr.replace(",}","}");
                linesGro.append("\naction("+celulasASgr+", \"emit_area\" , {tostring("+signalEit+"),\""+concentrationEmit+"\"});\n");

            }
            else
            {
                linesGro.append("\naction({"+cellAction+"}, \"emit_area\" , {tostring("+signalEit+"),\""+concentrationEmit+"\"});\n");
            }
        }
        //emit_cross_feeding
        cellAction = "";
        masDeUno = false;
        cellActionAux.clear();
        if(linesYaml[numeroLineaDeYaml].contains("emit_cross"))
        {
            QString concentrationEmit = "";
            QString signalEit = "";
            numeroLineaDeYaml++;
            while(linesYaml[numeroLineaDeYaml].contains("world_act") == false && linesYaml[numeroLineaDeYaml].contains("output") == false && linesYaml[numeroLineaDeYaml].contains("paint") == false && linesYaml[numeroLineaDeYaml].contains("die") == false && linesYaml[numeroLineaDeYaml].contains("conjugation") == false && linesYaml[numeroLineaDeYaml].contains("lose") == false && linesYaml[numeroLineaDeYaml].contains("entry_exclusion") == false && linesYaml[numeroLineaDeYaml].contains("groth_rate") == false && linesYaml[numeroLineaDeYaml].contains("emit") == false && linesYaml[numeroLineaDeYaml].contains("quorum") == false && linesYaml[numeroLineaDeYaml].contains("feeding") == false)
            {
                if(linesYaml[numeroLineaDeYaml].contains("condition"))
                {
                    if(linesYaml[numeroLineaDeYaml].contains("[]"))
                    {
                        cellActionAux.append("");
                    }
                    else
                    {
                        if(linesYaml[numeroLineaDeYaml].contains(","))
                        {
                            cellActionAux = linesYaml[numeroLineaDeYaml].replace("condition:[","").replace("]","").split(",");
                            masDeUno = true;
                        }
                        else
                        {
                            cellAction = linesYaml[numeroLineaDeYaml].replace("condition:[","").replace("]","");
                        }
                    }
                }
                if(linesYaml[numeroLineaDeYaml].contains("signal"))
                {
                    signalEit = linesYaml[numeroLineaDeYaml].replace("signal:","");
                }
                if(linesYaml[numeroLineaDeYaml].contains("concentration"))
                {
                    concentrationEmit = linesYaml[numeroLineaDeYaml].replace("concentration:","");
                }
                numeroLineaDeYaml++;
            }
            if(masDeUno)
            {
                QString celulasASgr = "";
                for(int i = 0; i < cellActionAux.size(); i++)
                {
                    celulasASgr = celulasASgr + "\""+cellActionAux[i]+"\",";
                }
                celulasASgr = "{"+celulasASgr+"}";
                celulasASgr.replace(",}","}");
                linesGro.append("\naction("+celulasASgr+", \"emit_cf\" , {tostring("+signalEit+"),\""+concentrationEmit+"\"});\n");
            }
            else
            {
                linesGro.append("\naction({"+cellAction+"}, \"emit_cf\" , {tostring("+signalEit+"),\""+concentrationEmit+"\"});\n");
            }

        }
        //read_quorum_sense
        cellAction = "";
        masDeUno = false;
        cellActionAux.clear();
        if(linesYaml[numeroLineaDeYaml].contains("read_quorum"))
        {
            QString signalEit = "";
            QString upperThree = "";
            QString lowerThree = "";
            QString expr = "";
            numeroLineaDeYaml++;
            while(linesYaml[numeroLineaDeYaml].contains("world_act") == false && linesYaml[numeroLineaDeYaml].contains("output") == false && linesYaml[numeroLineaDeYaml].contains("paint") == false && linesYaml[numeroLineaDeYaml].contains("die") == false && linesYaml[numeroLineaDeYaml].contains("conjugation") == false && linesYaml[numeroLineaDeYaml].contains("lose") == false && linesYaml[numeroLineaDeYaml].contains("entry_exclusion") == false && linesYaml[numeroLineaDeYaml].contains("groth_rate") == false && linesYaml[numeroLineaDeYaml].contains("emit") == false && linesYaml[numeroLineaDeYaml].contains("quorum") == false && linesYaml[numeroLineaDeYaml].contains("feeding") == false)
            {
                if(linesYaml[numeroLineaDeYaml].contains("condition"))
                {
                    if(linesYaml[numeroLineaDeYaml].contains("[]"))
                    {
                        cellActionAux.append("");
                    }
                    else
                    {
                        if(linesYaml[numeroLineaDeYaml].contains(","))
                        {
                            cellActionAux = linesYaml[numeroLineaDeYaml].replace("condition:[","").replace("]","").split(",");
                            masDeUno = true;
                        }
                        else
                        {
                            cellAction = linesYaml[numeroLineaDeYaml].replace("condition:[","").replace("]","");
                        }
                    }
                }
                if(linesYaml[numeroLineaDeYaml].contains("signal"))
                {
                    signalEit = linesYaml[numeroLineaDeYaml].replace("signal:","");
                }
                if(linesYaml[numeroLineaDeYaml].contains("upper_threshold"))
                {
                    upperThree = linesYaml[numeroLineaDeYaml].replace("upper_threshold:","");
                }
                if(linesYaml[numeroLineaDeYaml].contains("lower_threshold"))
                {
                    lowerThree = linesYaml[numeroLineaDeYaml].replace("lower_threshold:","");
                }
                if(linesYaml[numeroLineaDeYaml].contains("expresses"))
                {
                    expr = linesYaml[numeroLineaDeYaml].replace("expresses:","");
                }
                numeroLineaDeYaml++;
            }
            if(masDeUno)
            {
                QString celulasASgr = "";
                for(int i = 0; i < cellActionAux.size(); i++)
                {
                    celulasASgr = celulasASgr + "\""+cellActionAux[i]+"\",";
                }
                celulasASgr = "{"+celulasASgr+"}";
                celulasASgr.replace(",}","}");
                linesGro.append("\naction("+celulasASgr+", \"read_quorum_sense\" , {tostring("+signalEit+"),\""+upperThree+"\",\""+lowerThree+"\",\""+expr+"\"});\n");
            }
            else
            {
                linesGro.append("\naction("+cellAction+", \"read_quorum_sense\" , {tostring("+signalEit+"),\""+upperThree+"\",\""+lowerThree+"\",\""+expr+"\"});\n");
            }
        }
        //absorb_quoprum_sense
        cellAction = "";
        masDeUno = false;
        cellActionAux.clear();
        if(linesYaml[numeroLineaDeYaml].contains("read_quorum"))
        {
            QString signalEit = "";
            QString upperThree = "";
            QString lowerThree = "";
            QString expr = "";
            numeroLineaDeYaml++;
            while(linesYaml[numeroLineaDeYaml].contains("world_act") == false && linesYaml[numeroLineaDeYaml].contains("output") == false && linesYaml[numeroLineaDeYaml].contains("paint") == false && linesYaml[numeroLineaDeYaml].contains("die") == false && linesYaml[numeroLineaDeYaml].contains("conjugation") == false && linesYaml[numeroLineaDeYaml].contains("lose") == false && linesYaml[numeroLineaDeYaml].contains("entry_exclusion") == false && linesYaml[numeroLineaDeYaml].contains("groth_rate") == false && linesYaml[numeroLineaDeYaml].contains("emit") == false && linesYaml[numeroLineaDeYaml].contains("quorum") == false && linesYaml[numeroLineaDeYaml].contains("feeding") == false)
            {

                if(linesYaml[numeroLineaDeYaml].contains("condition"))
                {
                    if(linesYaml[numeroLineaDeYaml].contains("[]"))
                    {
                        cellActionAux.append("");
                    }
                    else
                    {
                        if(linesYaml[numeroLineaDeYaml].contains(","))
                        {
                            cellActionAux = linesYaml[numeroLineaDeYaml].replace("condition:[","").replace("]","").split(",");
                            masDeUno = true;
                        }
                        else
                        {
                            cellAction = linesYaml[numeroLineaDeYaml].replace("condition:[","").replace("]","");
                        }
                    }
                }
                if(linesYaml[numeroLineaDeYaml].contains("signal"))
                {
                    signalEit = linesYaml[numeroLineaDeYaml].replace("signal:","");
                }
                if(linesYaml[numeroLineaDeYaml].contains("upper_threshold"))
                {
                    upperThree = linesYaml[numeroLineaDeYaml].replace("upper_threshold:","");
                }
                if(linesYaml[numeroLineaDeYaml].contains("lower_threshold"))
                {
                    lowerThree = linesYaml[numeroLineaDeYaml].replace("lower_threshold:","");
                }
                if(linesYaml[numeroLineaDeYaml].contains("expresses"))
                {
                    expr = linesYaml[numeroLineaDeYaml].replace("expresses:","");
                }
                numeroLineaDeYaml++;
            }
            if(masDeUno)
            {
                QString celulasASgr = "";
                for(int i = 0; i < cellActionAux.size(); i++)
                {
                    celulasASgr = celulasASgr + "\""+cellActionAux[i]+"\",";
                }
                celulasASgr = "{"+celulasASgr+"}";
                celulasASgr.replace(",}","}");
                linesGro.append("\naction("+celulasASgr+", \"absorb_quorum_sense\" , {tostring("+signalEit+"),\""+upperThree+"\",\""+lowerThree+"\",\""+expr+"\"});\n");
            }
            else
            {
                linesGro.append("\naction("+cellAction+", \"absorb_quorum_sense\" , {tostring("+signalEit+"),\""+upperThree+"\",\""+lowerThree+"\",\""+expr+"\"});\n");
            }
        }
        //get
        cellAction = "";
        masDeUno = false;
        cellActionAux.clear();
        if(linesYaml[numeroLineaDeYaml].contains("get_cross"))
        {
            QString benef = "";
            QString concentrationEmit = "";
            QString signalEit = "";
            numeroLineaDeYaml++;
            while(linesYaml[numeroLineaDeYaml].contains("world_act") == false && linesYaml[numeroLineaDeYaml].contains("output") == false && linesYaml[numeroLineaDeYaml].contains("paint") == false && linesYaml[numeroLineaDeYaml].contains("die") == false && linesYaml[numeroLineaDeYaml].contains("conjugation") == false && linesYaml[numeroLineaDeYaml].contains("lose") == false && linesYaml[numeroLineaDeYaml].contains("entry_exclusion") == false && linesYaml[numeroLineaDeYaml].contains("groth_rate") == false && linesYaml[numeroLineaDeYaml].contains("emit") == false && linesYaml[numeroLineaDeYaml].contains("quorum") == false && linesYaml[numeroLineaDeYaml].contains("feeding") == false)
            {
                if(linesYaml[numeroLineaDeYaml].contains("condition"))
                {
                    if(linesYaml[numeroLineaDeYaml].contains("[]"))
                    {
                        cellActionAux.append("");
                    }
                    else
                    {
                        if(linesYaml[numeroLineaDeYaml].contains(","))
                        {
                            cellActionAux = linesYaml[numeroLineaDeYaml].replace("condition:[","").replace("]","").split(",");
                            masDeUno = true;
                        }
                        else
                        {
                            cellAction = linesYaml[numeroLineaDeYaml].replace("condition:[","").replace("]","");
                        }
                    }
                }
                if(linesYaml[numeroLineaDeYaml].contains("signal"))
                {
                    signalEit = linesYaml[numeroLineaDeYaml].replace("signal:","");
                }
                if(linesYaml[numeroLineaDeYaml].contains("concentration"))
                {
                    concentrationEmit = linesYaml[numeroLineaDeYaml].replace("concentration:","");
                }
                if(linesYaml[numeroLineaDeYaml].contains("benefit"))
                {
                    concentrationEmit = linesYaml[numeroLineaDeYaml].replace("benefit:","");
                }
                numeroLineaDeYaml++;
            }
            if(masDeUno)
            {
                QString celulasASgr = "";
                for(int i = 0; i < cellActionAux.size(); i++)
                {
                    celulasASgr = celulasASgr + "\""+cellActionAux[i]+"\",";
                }
                celulasASgr = "{"+celulasASgr+"}";
                celulasASgr.replace(",}","}");
                linesGro.append("\naction("+celulasASgr+", \"get_cf\" , {tostring("+signalEit+"),\""+concentrationEmit+"\",\""+benef+"\"});\n");
            }
            else
            {
                linesGro.append("\naction({"+cellAction+"}, \"get_cf\" , {tostring("+signalEit+"),\""+concentrationEmit+"\",\""+benef+"\"});\n");
            }

        }
        if(linesYaml[numeroLineaDeYaml].contains("world_act") == false && linesYaml[numeroLineaDeYaml].contains("output") == false && linesYaml[numeroLineaDeYaml].contains("paint") == false && linesYaml[numeroLineaDeYaml].contains("die") == false && linesYaml[numeroLineaDeYaml].contains("conjugation") == false && linesYaml[numeroLineaDeYaml].contains("lose") == false && linesYaml[numeroLineaDeYaml].contains("entry_exclusion") == false && linesYaml[numeroLineaDeYaml].contains("groth_rate") == false && linesYaml[numeroLineaDeYaml].contains("emit") == false && linesYaml[numeroLineaDeYaml].contains("quorum") == false && linesYaml[numeroLineaDeYaml].contains("feeding") == false)
        {
            numeroLineaDeYaml++;
        }
    }
    return numeroLineaDeYaml;
}
//metodo que se encarga de traducir la parte de world actions
int VentanaPrincipal::metodoWorldActions(int lineaActual)
{
    numeroLineaDeYaml = lineaActual;
    QStringList auxPlasmid;
    int contadorCepas = 0;
    int contadorSignals = 0;
    int contadore = 0;
    //cuenta el numero de cepas y el numero de señales
    while(linesYaml[lineaActual].contains("output") == false)
    {
        if(linesYaml[lineaActual].contains("strain"))
        {
            contadorCepas++;
        }
        if(linesYaml[lineaActual].contains("signal"))
        {
            contadorSignals++;
        }
        lineaActual++;
    }
    QString cepas[contadorCepas];
    QString program[contadorCepas];
    QString auxPlasmid2[contadorCepas];
    QString auxGrowth[contadorCepas];
    for(int i = 0; i < contadorCepas; i++)
    {
        cepas[i] = "";
        program[i] = "";
        auxPlasmid2[i] = "";
        auxGrowth[i] = "";
    }
    QString nombreCepa[contadorCepas];
    QString numero = "0";
    QString nombrePopulation[contadorCepas];
    QString time[contadorCepas];
    QString centroCirculo[contadorCepas][2];
    QString radioCirculo[contadorCepas];
    QString lineaStart[contadorCepas];
    QString lineaEnd[contadorCepas];
    QString lineaWidth[contadorCepas];
    QStringList aux;

    QString nombreSignal[contadorSignals];
    QString populationSignal[contadorSignals];
    QString positionSignal[contadorSignals];
    QStringList auxTimeSignal;
    QString timeSignal[contadorSignals][2];
    QString concentrationSignal[contadorSignals];
    QString refreshSignal[contadorSignals];
    int iCepa = 0;
    int iSignal = 0;
    for(int i = 0; i < contadorCepas ; i++)
    {
                nombreCepa[i] = "";
                time[i] = "";
                centroCirculo[i][0] = "";
                centroCirculo[i][1] = "";
                radioCirculo[i] = "";
                lineaStart[i] = "";
                lineaEnd[i] = "";
                lineaWidth[i] = "";
    }
    for(int i = 0; i < contadorSignals; i++)
    {
                nombreSignal[i] = "";
                qDebug() <<"nombre bien";
                positionSignal[i] = "";
                qDebug() <<"nombre posi";
                populationSignal[i] = "";
                qDebug() <<"nombre popu";
                timeSignal[i][0] = "";
                timeSignal[i][1] = "";
                qDebug() <<"nombre time";
                concentrationSignal[i] = "";
                qDebug() <<"nombre concen";
                refreshSignal[i] = "";
                qDebug() <<"nombre refresh";
    }
    //recorre world actions hasta llegar a output
    while(linesYaml[numeroLineaDeYaml].contains("output") == false)
    {
        //va guardando los atributos de señales y cepas en variables y dice si hay errores
        qDebug() << "primera linea del while "+linesYaml[numeroLineaDeYaml];
        if(linesYaml[numeroLineaDeYaml].contains("strain"))
        {
            nombreCepa[iCepa] = linesYaml[numeroLineaDeYaml].replace("-strain:","");
            qDebug() << "nombreCepa de world "+nombreCepa[iCepa];
            qDebug() << iCepa;

            numeroLineaDeYaml++;
            qDebug() << "linea antes del while y despues de strain "+linesYaml[numeroLineaDeYaml];
            while(linesYaml[numeroLineaDeYaml].contains("strain") == false && linesYaml[numeroLineaDeYaml].contains("signal") == false && linesYaml[numeroLineaDeYaml].contains("output") == false)
            {
                qDebug() << "entra en el while "+linesYaml[numeroLineaDeYaml];
                if(linesYaml[numeroLineaDeYaml].contains("population"))
                {
                    nombrePopulation[iCepa] = linesYaml[numeroLineaDeYaml].replace("population:","");
                    if(validaNumeroInt(nombrePopulation[iCepa]) == false)
                    {
                        numeroString = QString::number(numeroLineaDeYaml+1);
                        textoSemanticoCorrecto = false;
                        errorSemantico.append("\nERROR line "+numeroString+": in world_actions, in strain, population has to be number (int)\n");
                    }
                    numeroLineaDeYaml++;
                }
                if(linesYaml[numeroLineaDeYaml].contains("time"))
                {
                    time[iCepa] = linesYaml[numeroLineaDeYaml].replace("time:","");
                    numeroLineaDeYaml++;
                }
                qDebug() << "deberia ser circle "+linesYaml[numeroLineaDeYaml];
                if(linesYaml[numeroLineaDeYaml].contains("circle"))
                {
                    numeroLineaDeYaml++;
                    if(linesYaml[numeroLineaDeYaml].contains("center"))
                    {
                        linesYaml[numeroLineaDeYaml].replace("center:{x:","").replace("y:","").replace("}","");
                        aux.clear();
                        aux = linesYaml[numeroLineaDeYaml].split(",");
                        centroCirculo[iCepa][0] = aux[0];
                        centroCirculo[iCepa][1] = aux[1];
                        if(validaNumeroFloat(centroCirculo[iCepa][0]) == false || validaNumeroFloat(centroCirculo[iCepa][0]) == false)
                        {
                            numeroString = QString::number(numeroLineaDeYaml+1);
                            textoSemanticoCorrecto = false;
                            errorSemantico.append("\nERROR line "+numeroString+": in world actions, the center of circle have to be number(float)\n");
                        }
                        numeroLineaDeYaml++;
                    }
                    if(linesYaml[numeroLineaDeYaml].contains("radius"))
                    {
                        radioCirculo[iCepa] = linesYaml[numeroLineaDeYaml].replace("radius:","");
                        if(validaNumeroFloat(radioCirculo[iCepa]) == false)
                        {
                            numeroString = QString::number(numeroLineaDeYaml+1);
                            textoSemanticoCorrecto = false;
                            errorSemantico.append("\nERROR line "+numeroString+": in world actions, the radius of circle has to be number(float)\n");
                        }
                        numeroLineaDeYaml++;
                    }
                    qDebug() << "iCepa";
                    qDebug() << iCepa;
                }
                else if(linesYaml[numeroLineaDeYaml].contains("linear"))
                {
                    numeroLineaDeYaml++;
                    if(linesYaml[numeroLineaDeYaml].contains("start"))
                    {
                        lineaStart[iCepa] = linesYaml[numeroLineaDeYaml].replace("start:","");
                        if(validaNumeroFloat(lineaStart[iCepa]) == false)
                        {
                            numeroString = QString::number(numeroLineaDeYaml+1);
                            textoSemanticoCorrecto = false;
                            errorSemantico.append("\nERROR line "+numeroString+": in world actions, the start of linear has to be number(float)\n");
                        }
                        numeroLineaDeYaml++;
                    }
                    if(linesYaml[numeroLineaDeYaml].contains("end"))
                    {
                        lineaEnd[iCepa] = linesYaml[numeroLineaDeYaml].replace("end:","");
                        if(validaNumeroFloat(lineaEnd[iCepa]) == false)
                        {
                            numeroString = QString::number(numeroLineaDeYaml+1);
                            textoSemanticoCorrecto = false;
                            errorSemantico.append("\nERROR line "+numeroString+": in world actions, the end of linear has to be number(float)\n");
                        }
                        numeroLineaDeYaml++;
                    }
                    if(linesYaml[numeroLineaDeYaml].contains("width"))
                    {
                        lineaWidth[iCepa] = linesYaml[numeroLineaDeYaml].replace("width:","");
                        if(validaNumeroFloat(lineaWidth[iCepa]) == false)
                        {
                            numeroString = QString::number(numeroLineaDeYaml+1);
                            textoSemanticoCorrecto = false;
                            errorSemantico.append("\nERROR line "+numeroString+": in world actions, the width of linear has to be number(float)\n");
                        }
                        numeroLineaDeYaml++;
                    }
                    qDebug() << "iCepa";
                    qDebug() << iCepa;
                }
                qDebug() << "iCepa";
                qDebug() << iCepa;
            }
            iCepa++;
        }
        qDebug() << "ver porque linea va despues del strain de world "+linesYaml[numeroLineaDeYaml];
        if(linesYaml[numeroLineaDeYaml].contains("signal"))
        {
            qDebug()<< "entra en signal de world "+linesYaml[numeroLineaDeYaml];
            nombreSignal[iSignal] = linesYaml[numeroLineaDeYaml].replace("-signal:","");
            numeroLineaDeYaml++;
            while(linesYaml[numeroLineaDeYaml].contains("strain") == false && linesYaml[numeroLineaDeYaml].contains("signals") == false && linesYaml[numeroLineaDeYaml].contains("output") == false)
            {
                qDebug()<< "deberia ser la linea de despues de signal "+linesYaml[numeroLineaDeYaml];
                if(linesYaml[numeroLineaDeYaml].contains("population"))
                {
                    populationSignal[iSignal] = linesYaml[numeroLineaDeYaml].replace("population:","");
                    if(validaNumeroInt(populationSignal[iSignal]) == false)
                    {
                        numeroString = QString::number(numeroLineaDeYaml+1);
                        textoSemanticoCorrecto = false;
                        errorSemantico.append("\nERROR line "+numeroString+": in world_actions, in signal, population has to be number(int)\n");
                    }
                    numeroLineaDeYaml++;
                }
                if(linesYaml[numeroLineaDeYaml].contains("position"))
                {
                    positionSignal[iSignal] = linesYaml[numeroLineaDeYaml].replace("position:","");
                    numeroLineaDeYaml++;
                    qDebug() << "entro en position "+linesYaml[numeroLineaDeYaml];
                }
                if(linesYaml[numeroLineaDeYaml].contains("concentration"))
                {
                    concentrationSignal[iSignal] = linesYaml[numeroLineaDeYaml].replace("concentration:","");
                    if(validaNumeroFloat(concentrationSignal[iSignal]) == false)
                    {
                        numeroString = QString::number(numeroLineaDeYaml+1);
                        textoSemanticoCorrecto = false;
                        errorSemantico.append("\nERROR line "+numeroString+": in world actions, in signals, concentration has to be number (float)\n");
                    }
                    numeroLineaDeYaml++;
                    qDebug() << "entro en concentration "+linesYaml[numeroLineaDeYaml];
                }
                if(linesYaml[numeroLineaDeYaml].contains("refresh"))
                {
                    refreshSignal[iSignal] = linesYaml[numeroLineaDeYaml].replace("refresh:","");
                    if(validaNumeroFloat(refreshSignal[iSignal]) == false)
                    {
                        numeroString = QString::number(numeroLineaDeYaml+1);
                        textoSemanticoCorrecto = false;
                        errorSemantico.append("\nERROR line "+numeroString+": in world actions, in signals, refresh has to be number (float)\n");
                    }
                    numeroLineaDeYaml++;
                }
                if(linesYaml[numeroLineaDeYaml].contains("time"))
                {
                    auxTimeSignal.clear();
                    auxTimeSignal = linesYaml[numeroLineaDeYaml].replace("time:","").replace("[","").replace("]","").split(",");
                    timeSignal[iSignal][0] = auxTimeSignal[0];
                    timeSignal[iSignal][1] = auxTimeSignal[1];
                    numeroLineaDeYaml++;
                }

                qDebug() << "iSignal";
                qDebug() << iSignal;
            }
            iSignal++;
        }
        if(linesYaml[numeroLineaDeYaml].contains("strain") == false && linesYaml[numeroLineaDeYaml].contains("signal") == false && linesYaml[numeroLineaDeYaml].contains("output") == false && linesYaml[numeroLineaDeYaml].contains("population") == false && linesYaml[numeroLineaDeYaml].contains("time") == false && linesYaml[numeroLineaDeYaml].contains("circle") == false && linesYaml[numeroLineaDeYaml].contains("linear") == false)
        {
            numeroLineaDeYaml++;
        }

    }
    auxPlasmid.clear();
    //coincidir las cepas de strains con las definidas en world actions
    for(int k = 0; k < contadorCepas; k++)
    {
        for(int m = 0; m < nombreGlobalCepa.size(); m++)
        {
            if(nombreCepa[k] == nombreGlobalCepa[m])
            {
                contadore = 1;
            }
        }
        if(contadore == 0)
        {
            //si hay cepas en world acions que no estan definidas en cepas error
            numeroString = QString::number(numeroLineaDeYaml+1);
            textoSemanticoCorrecto = false;
            errorSemantico.append("\nERROR: There are strains in world actions that they weren't declared\n");
        }
        contadore = 0;
    }
    if(textoSemanticoCorrecto)
    {
        //coincidir strains con world_actions
        for(int i = 0; i < contadorCepas; i++)
        {
            for(int j = 0; j < nomCepas.size(); j++)
            {
                qDebug() << "nombre cepa i "+nombreCepa[i];
                qDebug() << "nombre cepa j "+nomCepas[j];
                if(nombreCepa[i] == nomCepas[j])
                {
                    qDebug()<< "nombre de cepas cuando ya coinciden "+nombreCepa[i];
                    auxPlasmid2[i] = plCepas[j];
                    qDebug() << "nombre de plasmidos "+auxPlasmid2[i];
                    auxGrowth[i] = radCrecCep[j];
                    qDebug() << "nombreradio crec "+auxGrowth[i];
                }
            }
        }
        //gfuardar los datos en variables globales para traducirlas en el metodo output
        for(int i = 0; i < contadorCepas; i++)
        {
            nCepas.append(nombreCepa[i]);
            nombrePopul.append(nombrePopulation[i]);
            centroCirculo1.append(centroCirculo[i][0]);
            centroCirculo2.append(centroCirculo[i][1]);
            radioCircul.append(radioCirculo[i]);
            auPl2.append(auxPlasmid2[i]);
            auGr.append(auxGrowth[i]);
        }
    }
    //idem que cepas pero de señales
    for(int k = 0; k < contadorSignals; k++)
    {
        for(int m = 0; m < nombreGlobalSignal.size(); m++)
        {
            if(nombreSignal[k] == nombreGlobalSignal[m])
            {
                contadore = 1;
            }
        }
        if(contadore == 0)
        {
            numeroString = QString::number(numeroLineaDeYaml+1);
            textoSemanticoCorrecto = false;
            errorSemantico.append("\nERROR: There are signals in world actions that they weren't declared\n");
        }
        contadore = 0;
    }
    return numeroLineaDeYaml;
}
//metodo que se encarga de traducir la parte de output
int VentanaPrincipal::metodoOutput(int lineaActual)
{
    int numFicheros = 0;
    QString numero = "0";
    numeroLineaDeYaml = lineaActual;
    linesYaml.append("");
    int lineaAux = 0;
    //cuenta el numero de ficheros
    while(lineaActual < linesYaml.size()-1)
    {
        if(linesYaml[lineaActual].contains("file"))
        {
            numFicheros++;
        }
        lineaActual++;
    }
    qDebug()<<"linea 1814 "+linesYaml[numeroLineaDeYaml];
    QString rutaFicherosSalida[numFicheros][2];
    for(int i = 0; i < numFicheros; i++)
    {
        rutaFicherosSalida[i][0] = "";
        rutaFicherosSalida[i][1] = "";
    }
    qDebug()<<"linea 1821 "+linesYaml[numeroLineaDeYaml];
    //almacerna los atributos en variables
    while(numeroLineaDeYaml < linesYaml.size() - 2)
    {
        if(numeroLineaDeYaml < linesYaml.size()-2)
        {
            numeroLineaDeYaml++;
        }
        qDebug()<<"linea 1828 "+linesYaml[numeroLineaDeYaml];
        if(linesYaml[numeroLineaDeYaml].contains("dump"))
        {
            while(linesYaml[numeroLineaDeYaml].contains("picture") == false && numeroLineaDeYaml < linesYaml.size() - 2)
            {
                qDebug() << numeroLineaDeYaml;
                qDebug() << "numero linea yaml ";
                qDebug() <<linesYaml.size();
                if(linesYaml[numeroLineaDeYaml].contains("condition"))
                {
                    qDebug() << "linea 1835 "+linesYaml[numeroLineaDeYaml];
                    numeroLineaDeYaml++;
                    if(linesYaml[numeroLineaDeYaml].contains("file"))
                    {
                        rutaFicherosSalida[lineaAux][0] = linesYaml[numeroLineaDeYaml].replace("file:","");
                        lineaAux++;
                        qDebug() << "linea 1941"+linesYaml[numeroLineaDeYaml];
                    }
                }
                qDebug() << "linea 1944"+linesYaml[numeroLineaDeYaml];
                if(numeroLineaDeYaml < linesYaml.size()-2)
                {
                    numeroLineaDeYaml++;
                }
                qDebug() << "linea 1946"+linesYaml[numeroLineaDeYaml];
            }

        }
        else if(linesYaml[numeroLineaDeYaml].contains("picture"))
        {
            numeroLineaDeYaml++;
        }
        qDebug() << "linea 1954"+linesYaml[numeroLineaDeYaml];
    }
    qDebug()<<"linea 1849 "+linesYaml[numeroLineaDeYaml];
    int posUltimaBarra[numFicheros];
    for(int i = 0; i < numFicheros; i++)
    {
        for(int j = rutaFicherosSalida[i][0].length()-1; j >= 0; j--)
        {
            if(rutaFicherosSalida[i][0][j] == '/')
            {
                posUltimaBarra[i] = j;
                j = -1;
            }
        }
    }
    //traduce
    QString stringAux = "";
    QString stringAux2 = "";
    for(int i = 0; i < numFicheros; i++)
    {
        stringAux = rutaFicherosSalida[i][0];
        stringAux2 = rutaFicherosSalida[i][0];
        rutaFicherosSalida[i][0] = stringAux.remove(posUltimaBarra[i]+1,stringAux.count());
        qDebug() << rutaFicherosSalida[i][0];
        rutaFicherosSalida[i][1] = stringAux2.remove(0,posUltimaBarra[i]+1);
        qDebug() << stringAux+" el aux y el aux 2 "+stringAux2;
        qDebug() << stringAux2;
    }
    for(int i = 0; i < numFicheros; i++)
    {
        numero = QString::number(i+1);
        linesGro.append("\nroute"+numero+" := \""+rutaFicherosSalida[i][0]+"\";\n");
    }
    qDebug() << "linea 1892";
    for(int i = 0; i < numFicheros; i++)
    {
        numero = QString::number(i+1);
        linesGro.append("\nfilename"+numero+" := \""+rutaFicherosSalida[i][1]+"\";\n");
    }
    qDebug() << "linea 1898";
    for(int i = 0; i < numFicheros; i++)
    {
        numero = QString::number(i+1);
        linesGro.append("\nfp"+numero+" := fopen ( route"+numero+" <> filename"+numero+", \"w\" );\n");
    }
    qDebug() << "linea 1904";
    if(textoSemanticoCorrecto)
    {
        for(int i = 0; i < contCepas; i++)
        {
            numero = QString::number(i+1);
            cepas.append("\n\tc_ecolis("+nombrePopul[i]+", "+centroCirculo1[i]+", "+centroCirculo2[i]+", "+radioCircul[i]+", {\""+auPl2[i]+"\"}, program p"+numero+"())\n");
            program.append("\nprogram p"+numero+"() := \n{ \n\tset (\""+nCepas[i]+"_growth_rate\", "+auGr[i]+");\n\tselected:\n\t{\n\t\tdump_single(fp"+numero+");\n\t} \n};\n");
        }
        qDebug() << "numero de liles "+linesYaml[lineaActual]+ " la actual  y el num linea "+linesYaml[numeroLineaDeYaml];
        qDebug() << numFicheros;
        for(int i = 0; i < program.size(); i++)
        {
            linesGro.append(program[i]);
        }
        QString elementos = "";
        QString elementosNegativos = "";
        for(int i = 0; i < numGeneticsElement; i++)
        {
            elementos = elementos + nombreDeTodosLosElementos[i]+", ";
        }
        QString fprin = "";
        for(int i = 0; i < numFicheros; i++)
        {
            numero = QString::number(i+1);
            fprin = fprin +"\n\t\tfprint(fp"+numero+", \"Time, No protein, "+elementos+" Total\\n\");\n";
            fprin = fprin.replace(", ,",",");
        }
        elementos = "";
        for(int i = 0; i < numGeneticsElement; i++)
        {
            elementosNegativos = elementosNegativos + "\"-"+nombreDeTodosLosElementos[i]+"\",";
            elementos = elementos + "{\""+nombreDeTodosLosElementos[i]+"\"},";
        }
        elementosNegativos = "{"+elementosNegativos+"},"+elementos+"{});\n";
        fprin = "\tt = 0:\n\t{\n\t\t"+fprin+"\t}\n";
        linesGro.append("\nprogram main() := \n{"+fprin);
        QString dumpMultiple = "";
        for(int i = 0; i< numFicheros; i++)
        {
            numero = QString::number(i+1);
            dumpMultiple = dumpMultiple+"\n\t\tdump_multiple(fp"+numero+","+elementosNegativos;
        }
        linesGro.append("\ttrue:\n\t{\n\t\tt := t + dt;\n\t\t"+dumpMultiple+"\n\t}\n");
        for(int i = 0; i < contCepas; i++)
        {
            linesGro.append(cepas[i]);
        }
        linesGro.append("\n\tselect_random_cell();\n};\n");

    }

    return numeroLineaDeYaml;
}
//metodo al pulsar el boton traducir
void VentanaPrincipal::on_botonTraducir_clicked()
{
    numeroLineaDeYaml = 0;
    numeroLineaGro = 0;
    ui->textoGro->clear();
    QFile archivoYaml(path);
    ui->textoGro->setEnabled(true);
    archivoYaml.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream s(&archivoYaml);
    s<<ui->textoyaml->toPlainText();
    int numLineasTextoYaml = 0;
    int posAlmohadilla = 0;
    //bucle para rellenar linesYaml con las lineas del fichero yaml quitando
    //los comentarios, tabuladores y espacios en blanco. Cada linea va en una
    //posicion del array
    while(true)
    {
        QString aux = s.readLine();
        if(aux.isNull())
            break;
        else
        {
            numLineasTextoYaml = numLineasTextoYaml + 1;
            aux = aux.replace(" ","").replace("\t","").replace("\n","");
            if(aux.contains("#"))
            {
                posAlmohadilla = aux.indexOf("#");
                aux = aux.remove(posAlmohadilla,aux.size()-1);
            }
            if(aux.isEmpty()==false)
                linesYaml.append(aux);
        }
    }
    //vuelve a empezar a leer
    archivoYaml.seek(0);
    //cierro el fichero
    archivoYaml.close();
    linesGro.append("include gro\n\n");
    qDebug() << "linea357";
    //bucle que comprueba si estan las seis partes del fichero yaml, unas son obligatorias
    //otras no porque se ponen valores por defecto y otras tampoco son obligatorias y se
    //omiten al traducir
    for(int i = 0; i < linesYaml.size(); i++)
    {
        if(linesYaml[i].contains("simulation"))
        {
            boolSimulations = true;
        }
        if(linesYaml[i].contains("signals"))
        {
            boolSignals = true;
        }
        if(linesYaml[i].contains("genetics"))
        {
            boolGenetics = true;
        }
        if(linesYaml[i].contains("strains"))
        {
            boolStrains = true;
        }
        if(linesYaml[i].contains("cell_action"))
        {
            boolCellActions = true;
        }
        if(linesYaml[i].contains("world"))
        {
            boolWorldActions = true;
        }
        if(linesYaml[i].contains("output"))
        {
            boolOutput = true;
        }
    }
    if(boolSimulations == false)
    {
        linesGro.append("\nset(\"dt\", 0.1);\n");
        linesGro.append("\nset(\"population_max\", 2000000);\n");
    }
    if(boolSignals == false)
    {
        linesGro.append("\ngrid ( \"continuous\", \"gro_original\", 10, 10, 8);\n");
    }
    if(boolGenetics == false)
    {
        textoSemanticoCorrecto = false;
        errorSemantico.append(("\nERROR: \"genetics\" not found\n"));
    }
    if(boolOutput == false)
    {
            textoSemanticoCorrecto = false;
            errorSemantico.append(("\nERROR: \"output\" not found\n"));
    }
    //recorre linesYaml hasta encontrar una parte, una vez encontrada accede a su metodo
    for(int i = 0; i < linesYaml.size(); i++)
    {
        if(linesYaml[i].contains("simulation"))
        {
            i = metodoSimulation(i);
        }
        qDebug()<<"pasa simulation";
        if(linesYaml[i].contains("signals"))
        {
            i = metodoSignals(i);
        }
        qDebug()<<"pasa signal";
        if(linesYaml[i].contains("genetics"))
        {
            i = metodoGenetics(i);
        }
        qDebug()<<"pasa genetics";
        if(linesYaml[i].contains("strains"))
        {
            i = metodoStrain(i);
        }
        qDebug()<<"pasa strains";
        if(linesYaml[i].contains("cell_action"))
        {
            i = metodoCellActions(i);
        }
        qDebug()<<"pasa cell";
        if(linesYaml[i].contains("world"))
        {
            i = metodoWorldActions(i);
        }
        qDebug()<<"pasa world";
        if(linesYaml[i].contains("output"))
        {
            i = metodoOutput(i);
        }
        qDebug()<<"pasa output";
    }
    qDebug() << "linea despues de bucle for de los metodos de simulations signals etc";
    qDebug() << "1 "+textoPonerEnGro;
    //si no hay errores rellenar el QString textoPonerEnGro
    if(textoSemanticoCorrecto)
    {
        QString textoGro2 = "";
        for(int i = 0; i < linesGro.size(); i++)
        {
            textoGro2 = textoGro2 + linesGro[i];
        }
        qDebug() << "entra en este if "+textoGro2;
        textoPonerEnGro = textoGro2;
    }
    else
    {
        QString textoGroErrores = "";
        for(int i = 0; i < errorSemantico.size(); i++)
        {
            textoGroErrores = textoGroErrores+errorSemantico[i];
        }
        qDebug() << "entra en este else "+textoGroErrores;
        textoPonerEnGro = textoGroErrores;
    }
    qDebug() << "2 "+textoPonerEnGro;
    escribirEnGro();
}

