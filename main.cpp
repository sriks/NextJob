#include <QtGui/QApplication>
#include <QtDeclarative>
#include "qmlapplicationviewer.h"
#include "JobManager.h"
#include "JobModel.h"
#include "JobAlert.h"
#include "AlertModel.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));
#if defined(TEST_WITH_DUMMY_DATA)
    qWarning()<<"*********Running with dummy data*************";
#endif
    QmlApplicationViewer viewer;
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    qmlRegisterType<JobModel>();
    qmlRegisterType<JobAlert>();
    qmlRegisterType<AlertModel>();
    qmlRegisterType<JobManager>();
    JobManager* mgr = JobManager::instance();
    mgr->setParent(QCoreApplication::instance());
    viewer.rootContext()->setContextProperty("njengine",(QObject*)mgr);
    viewer.setMainQmlFile(QLatin1String("qml/NextJob/main.qml"));
    viewer.showExpanded();
    return app->exec();
}
