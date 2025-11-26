"""
测试记录器
协调测试运行和数据库存储
"""
from pathlib import Path
from typing import List
from core.database import TestDatabase, TestRun, TestCaseDetail
from core.qt_project.unit_test_runner import TestResult, TestCaseResult
from core.qt_project.ui_test_runner import UITestResult, UITestScreenshot
from core.utils.logger import logger


class TestRecorder:
    """测试记录器"""
    
    def __init__(self, db: TestDatabase):
        """
        初始化记录器
        
        Args:
            db: 数据库实例
        """
        self.db = db
    
    def record_unit_test(
        self, 
        project_path: str, 
        result: TestResult,
        ai_analysis: str = None
    ) -> int:
        """
        记录单元测试结果
        
        Args:
            project_path: 项目路径
            result: 测试结果
            ai_analysis: AI 分析报告（可选）
            
        Returns:
            run_id: 记录 ID
        """
        logger.info(f"记录单元测试: {result.test_name}")
        
        # 创建测试运行记录
        run = TestRun(
            project_path=project_path,
            test_name=result.test_name,
            test_type='unit',
            status=result.status,
            total=result.total,
            passed=result.passed,
            failed=result.failed,
            skipped=result.skipped,
            duration=result.duration,
            output=result.output,
            ai_analysis=ai_analysis
        )
        
        # 保存到数据库
        run_id = self.db.save_test_run(run)
        
        # 保存测试用例详情
        details = [
            TestCaseDetail(
                run_id=run_id,
                case_name=case.name,
                status=case.status,
                message=case.message
            )
            for case in result.details
        ]
        self.db.save_test_case_details(run_id, details)
        
        logger.info(f"单元测试记录完成: run_id={run_id}")
        return run_id
    
    def record_ui_test(
        self, 
        project_path: str, 
        result: UITestResult,
        ai_analysis: str = None
    ) -> int:
        """
        记录 UI 测试结果（含截图）
        
        Args:
            project_path: 项目路径
            result: UI 测试结果
            ai_analysis: AI 分析报告（可选）
            
        Returns:
            run_id: 记录 ID
        """
        logger.info(f"记录 UI 测试: {result.test_name}")
        
        # 创建测试运行记录
        run = TestRun(
            project_path=project_path,
            test_name=result.test_name,
            test_type='ui',
            status=result.status,
            total=result.total,
            passed=result.passed,
            failed=result.failed,
            skipped=result.skipped,
            duration=result.duration,
            output=result.output,
            ai_analysis=ai_analysis
        )
        
        # 保存到数据库
        run_id = self.db.save_test_run(run)
        
        # 保存截图到数据库并删除文件
        self._save_screenshots_to_db(run_id, result.screenshots)
        
        logger.info(f"UI 测试记录完成: run_id={run_id}, 截图数={len(result.screenshots)}")
        return run_id
    
    def update_ai_analysis(self, run_id: int, analysis: str):
        """
        更新 AI 分析报告
        
        Args:
            run_id: 测试运行 ID
            analysis: AI 分析内容
        """
        self.db.update_ai_analysis(run_id, analysis)
        logger.info(f"更新 AI 分析: run_id={run_id}")
    
    def _save_screenshots_to_db(self, run_id: int, screenshots: List[UITestScreenshot]):
        """
        保存截图到数据库并删除文件
        
        Args:
            run_id: 测试运行 ID
            screenshots: 截图列表
        """
        for screenshot in screenshots:
            try:
                # 读取图片文件
                image_path = Path(screenshot.file_path)
                if not image_path.exists():
                    logger.warning(f"截图文件不存在: {image_path}")
                    continue
                
                with open(image_path, 'rb') as f:
                    image_data = f.read()
                
                # 保存到数据库
                self.db.save_screenshot(
                    run_id=run_id,
                    step_number=screenshot.step_number,
                    step_name=screenshot.step_name,
                    image_data=image_data
                )
                
                # 删除文件
                image_path.unlink()
                logger.debug(f"截图已入库并删除: {image_path.name}")
                
            except Exception as e:
                logger.error(f"处理截图失败: {screenshot.file_path}, 错误: {e}")
        
        # 尝试删除截图目录（如果为空）
        try:
            screenshot_dir = Path(screenshots[0].file_path).parent
            if screenshot_dir.exists() and not any(screenshot_dir.iterdir()):
                screenshot_dir.rmdir()
                logger.info(f"已删除空截图目录: {screenshot_dir}")
        except Exception as e:
            logger.debug(f"删除截图目录失败: {e}")
