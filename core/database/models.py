"""
数据模型
"""
from dataclasses import dataclass, asdict
from typing import Optional, List
from datetime import datetime
import base64


@dataclass
class TestRun:
    """测试运行记录"""
    id: Optional[int] = None
    project_path: str = ""
    test_name: str = ""
    test_type: str = ""  # 'unit' | 'ui'
    status: str = ""  # 'passed' | 'failed' | 'error'
    total: int = 0
    passed: int = 0
    failed: int = 0
    skipped: int = 0
    duration: str = ""
    output: str = ""
    ai_analysis: Optional[str] = None  # AI 分析报告
    created_at: Optional[str] = None
    
    def to_dict(self):
        return asdict(self)


@dataclass
class TestCaseDetail:
    """测试用例详情"""
    id: Optional[int] = None
    run_id: int = 0
    case_name: str = ""
    status: str = ""  # 'PASS' | 'FAIL' | 'SKIP'
    message: Optional[str] = None
    
    def to_dict(self):
        return asdict(self)


@dataclass
class Screenshot:
    """测试截图"""
    id: Optional[int] = None
    run_id: int = 0
    step_number: int = 0
    step_name: str = ""
    image_data: Optional[bytes] = None
    created_at: Optional[str] = None
    
    def to_dict(self):
        """转换为字典（不含图片数据）"""
        return {
            'id': self.id,
            'run_id': self.run_id,
            'step_number': self.step_number,
            'step_name': self.step_name,
            'created_at': self.created_at
        }
    
    def to_base64_dict(self):
        """转换为字典（含 base64 图片）"""
        result = self.to_dict()
        if self.image_data:
            result['image_data'] = base64.b64encode(self.image_data).decode('utf-8')
        return result


@dataclass
class TestRunDetail:
    """测试运行详情（含用例和截图）"""
    run: TestRun
    details: List[TestCaseDetail]
    screenshots: List[Screenshot]
    
    def to_dict(self):
        return {
            **self.run.to_dict(),
            'details': [d.to_dict() for d in self.details],
            'screenshots': [s.to_base64_dict() for s in self.screenshots]
        }
