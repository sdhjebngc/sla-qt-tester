#include <QtTest/QtTest>
#include <QGraphicsScene>

class TestDiagramScene : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    // 简单测试用例
    void testSceneCreation();
    void testSceneRect();
};

void TestDiagramScene::initTestCase()
{
    qDebug() << "=== 开始 QGraphicsScene 测试 ===";
}

void TestDiagramScene::cleanupTestCase()
{
    qDebug() << "=== 完成 QGraphicsScene 测试 ===";
}

void TestDiagramScene::testSceneCreation()
{
    // 测试：创建场景
    QGraphicsScene *scene = new QGraphicsScene();
    QVERIFY(scene != nullptr);
    QCOMPARE(scene->items().count(), 0);
    
    delete scene;
    qDebug() << "✓ QGraphicsScene 创建成功";
}

void TestDiagramScene::testSceneRect()
{
    // 测试：场景矩形区域
    QGraphicsScene *scene = new QGraphicsScene();
    QRectF rect(0, 0, 800, 600);
    scene->setSceneRect(rect);
    
    QCOMPARE(scene->sceneRect(), rect);
    QCOMPARE(scene->width(), 800.0);
    QCOMPARE(scene->height(), 600.0);
    
    delete scene;
    qDebug() << "✓ QGraphicsScene 矩形测试通过";
}

QTEST_MAIN(TestDiagramScene)
#include "test_diagramscene.moc"
