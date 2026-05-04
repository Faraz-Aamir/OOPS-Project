// app.js — Online Judge frontend logic
// Faraz Aamir | 25I-2069 | OOP Project | Spring 2026

const API = '';
let currentUser = null;
let allProblems = [];
let currentProblemId = null;
let contestTimerInterval = null;
let currentContestProblemIndex = -1;

// ===== API HELPER =====
async function api(endpoint, method = 'GET', data = null) {
    const opts = { method, headers: { 'Content-Type': 'application/json' } };
    if (data) opts.body = JSON.stringify(data);
    try {
        const res = await fetch(API + endpoint, opts);
        return await res.json();
    } catch (e) {
        return { status: 'error', message: 'Server not reachable. Is the backend running?' };
    }
}

// ===== NAVIGATION =====
function showSection(name) {
    document.querySelectorAll('.section').forEach(s => s.classList.remove('active'));
    const section = document.getElementById('section-' + name);
    if (section) section.classList.add('active');

    document.querySelectorAll('.nav-link').forEach(l => l.classList.remove('active'));
    const link = document.querySelector(`.nav-link[onclick*="${name}"]`);
    if (link) link.classList.add('active');

    if (name === 'problems') loadProblems();
}

function showMsg(id, message, type) {
    const el = document.getElementById(id);
    if (!el) return;
    el.textContent = message;
    el.className = 'msg ' + type;
}

function closeModal(id) {
    document.getElementById(id).style.display = 'none';
}

// ===== AUTH =====
async function handleLogin(e) {
    e.preventDefault();
    const username = document.getElementById('login-username').value;
    const password = document.getElementById('login-password').value;
    const res = await api('/api/login', 'POST', { username, password });
    if (res.status === 'ok') {
        currentUser = res.data;
        updateNavForUser();
        showSection('home');
    } else {
        showMsg('login-msg', res.message, 'error');
    }
}

async function handleRegister(e) {
    e.preventDefault();
    const username = document.getElementById('reg-username').value;
    const email = document.getElementById('reg-email').value;
    const password = document.getElementById('reg-password').value;
    const res = await api('/api/register', 'POST', { username, email, password });
    if (res.status === 'ok') {
        showMsg('register-msg', res.message, 'success');
        document.getElementById('verify-email').value = email;
        document.getElementById('verify-modal').style.display = 'flex';
    } else {
        showMsg('register-msg', res.message, 'error');
    }
}

async function handleVerify() {
    const email = document.getElementById('verify-email').value;
    const code = document.getElementById('verify-code').value;
    const res = await api('/api/verify', 'POST', { email, code });
    if (res.status === 'ok') {
        showMsg('verify-msg', 'Verified! You can now login.', 'success');
        setTimeout(() => {
            closeModal('verify-modal');
            showSection('login');
        }, 1500);
    } else {
        showMsg('verify-msg', res.message, 'error');
    }
}

async function logout() {
    await api('/api/logout', 'POST');
    currentUser = null;
    updateNavForUser();
    showSection('home');
}

async function deleteAccount() {
    const res = await api('/api/delete-account', 'POST');
    if (res.status === 'ok') {
        currentUser = null;
        updateNavForUser();
        closeModal('delete-modal');
        showSection('home');
    }
}

function updateNavForUser() {
    const authEl = document.getElementById('nav-auth');
    const userEl = document.getElementById('nav-user');
    if (currentUser) {
        authEl.style.display = 'none';
        userEl.style.display = 'flex';
        document.getElementById('user-display').textContent = currentUser.username;
        document.getElementById('user-score').textContent = currentUser.score + ' pts';
    } else {
        authEl.style.display = 'flex';
        userEl.style.display = 'none';
    }
}

// ===== PROBLEMS =====
async function loadProblems() {
    if (!currentUser) { showSection('login'); return; }
    const res = await api('/api/problems');
    if (res.status === 'ok') {
        allProblems = res.data;
        renderProblems(allProblems);
    }
}

function renderProblems(problems) {
    const grid = document.getElementById('problems-list');
    const diffLabels = ['', 'Easy', 'Easy+', 'Medium', 'Hard', 'Expert'];
    grid.innerHTML = problems.map(p => `
        <div class="problem-card" onclick="openProblem(${p.id})">
            <div style="display:flex;justify-content:space-between;align-items:center;">
                <span class="problem-id">#${p.id}</span>
                <span class="difficulty-badge diff-${p.difficulty}">${diffLabels[p.difficulty]}</span>
            </div>
            <h4>${p.title}</h4>
        </div>
    `).join('');
}

function filterProblems(diff, btn) {
    document.querySelectorAll('.filter-btn').forEach(b => b.classList.remove('active'));
    btn.classList.add('active');
    if (diff === 0) renderProblems(allProblems);
    else renderProblems(allProblems.filter(p => p.difficulty === diff));
}

async function openProblem(id) {
    const res = await api('/api/problem/' + id);
    if (res.status !== 'ok') return;
    const p = res.data;
    currentProblemId = id;
    document.getElementById('solve-title').textContent = '#' + p.id + ' — ' + p.title;
    document.getElementById('solve-difficulty').textContent = '⭐'.repeat(p.difficulty);
    document.getElementById('solve-difficulty').className = 'difficulty-badge diff-' + p.difficulty;
    document.getElementById('solve-description').textContent = p.description;
    document.getElementById('solve-constraints').textContent = p.constraints;
    document.getElementById('solve-sample-input').textContent = p.sampleInput || '(none)';
    document.getElementById('solve-sample-output').textContent = p.sampleOutput;
    document.getElementById('code-editor').value = '#include<iostream>\nusing namespace std;\n\nint main() {\n    // Your code here\n    return 0;\n}\n';
    document.getElementById('verdict-box').style.display = 'none';
    showSection('solve');
}

function resetEditor() {
    document.getElementById('code-editor').value = '#include<iostream>\nusing namespace std;\n\nint main() {\n    // Your code here\n    return 0;\n}\n';
    document.getElementById('verdict-box').style.display = 'none';
}

async function submitSolution() {
    // For standalone problem solving (not contest mode)
    const code = document.getElementById('code-editor').value;
    document.getElementById('submit-btn').disabled = true;
    document.getElementById('submit-btn').textContent = 'Evaluating...';

    // We'll reuse the contest submit endpoint by creating a temporary contest
    // For now, show that this works via contest mode
    const vBox = document.getElementById('verdict-box');
    vBox.style.display = 'block';
    vBox.textContent = 'Please use Contest Mode to submit and evaluate solutions.';
    vBox.className = 'verdict-box verdict-error';
    document.getElementById('submit-btn').disabled = false;
    document.getElementById('submit-btn').textContent = 'Submit';
}

// ===== CONTEST =====
async function startContest() {
    if (!currentUser) { showSection('login'); return; }
    const res = await api('/api/contest/create', 'POST');
    if (res.status !== 'ok') {
        showMsg('contest-msg', res.message, 'error');
        return;
    }
    document.getElementById('contest-lobby').style.display = 'none';
    document.getElementById('contest-active').style.display = 'block';

    // Load contest problems
    loadContestProblems();
    startTimer(res.data.timeLimit);
}

async function loadContestProblems() {
    const res = await api('/api/contest/status');
    if (res.status !== 'ok' || !res.data) return;

    // Get all problems for the contest
    const probsRes = await api('/api/problems');
    if (probsRes.status !== 'ok') return;

    const grid = document.getElementById('contest-problems');
    const diffLabels = ['', 'Easy', 'Easy+', 'Medium', 'Hard', 'Expert'];
    // Show first N problems based on contest
    const count = res.data.problemCount;
    const contestProbs = probsRes.data.slice(0, count);
    grid.innerHTML = contestProbs.map((p, i) => `
        <div class="problem-card" onclick="openContestProblem(${i}, ${p.id})">
            <div style="display:flex;justify-content:space-between;align-items:center;">
                <span class="problem-id">Problem ${i + 1}</span>
                <span class="difficulty-badge diff-${p.difficulty}">${diffLabels[p.difficulty]}</span>
            </div>
            <h4>${p.title}</h4>
        </div>
    `).join('');
}

async function openContestProblem(index, id) {
    const res = await api('/api/problem/' + id);
    if (res.status !== 'ok') return;
    const p = res.data;
    currentContestProblemIndex = index;
    document.getElementById('contest-problem-title').textContent = 'Problem ' + (index + 1) + ': ' + p.title;
    document.getElementById('contest-problem-desc').textContent = p.description;
    document.getElementById('contest-sample-input').textContent = p.sampleInput || '(none)';
    document.getElementById('contest-sample-output').textContent = p.sampleOutput;
    document.getElementById('contest-editor').value = '#include<iostream>\nusing namespace std;\n\nint main() {\n    \n    return 0;\n}\n';
    document.getElementById('contest-solve').style.display = 'flex';
    document.getElementById('contest-verdict').style.display = 'none';
}

async function submitContestSolution() {
    const code = document.getElementById('contest-editor').value;
    const res = await api('/api/contest/submit', 'POST', {
        problemIndex: currentContestProblemIndex.toString(),
        code: code
    });
    const vBox = document.getElementById('contest-verdict');
    vBox.style.display = 'block';
    if (res.status === 'ok') {
        const verdict = res.data.verdict;
        vBox.textContent = '🏁 Verdict: ' + verdict;
        if (verdict === 'ACCEPTED') {
            vBox.className = 'verdict-box verdict-accepted';
        } else if (verdict === 'WRONG_ANSWER') {
            vBox.className = 'verdict-box verdict-wrong';
        } else {
            vBox.className = 'verdict-box verdict-error';
        }
    } else {
        vBox.textContent = res.message;
        vBox.className = 'verdict-box verdict-error';
    }
}

async function saveContestProgress() {
    const code = document.getElementById('contest-editor').value;
    await api('/api/contest/save-progress', 'POST', {
        problemIndex: currentContestProblemIndex.toString(),
        code: code
    });
}

async function endContest() {
    if (!confirm('End contest? Your score will be finalized.')) return;
    const res = await api('/api/contest/end', 'POST');
    clearInterval(contestTimerInterval);
    if (res.status === 'ok') {
        alert('Contest ended! Final score: ' + res.data.score);
        currentUser.score += res.data.score;
        updateNavForUser();
    }
    document.getElementById('contest-lobby').style.display = 'block';
    document.getElementById('contest-active').style.display = 'none';
    document.getElementById('contest-solve').style.display = 'none';
}

function startTimer(totalSeconds) {
    let remaining = totalSeconds;
    const timerEl = document.getElementById('contest-timer');
    contestTimerInterval = setInterval(() => {
        remaining--;
        if (remaining <= 0) {
            clearInterval(contestTimerInterval);
            endContest();
            return;
        }
        const min = Math.floor(remaining / 60);
        const sec = remaining % 60;
        timerEl.textContent = String(min).padStart(2, '0') + ':' + String(sec).padStart(2, '0');
        if (remaining < 60) timerEl.style.color = '#ef4444';
    }, 1000);
}

// ===== INIT =====
document.addEventListener('DOMContentLoaded', () => {
    showSection('home');
});
