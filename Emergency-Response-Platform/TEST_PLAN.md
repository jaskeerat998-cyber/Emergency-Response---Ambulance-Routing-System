# Manual acceptance test plan
1. Build using `bash build.sh` or `.\build.ps1`.
2. Register an incident near 30.3256, 78.0413; verify an ID is returned.
3. Register a severity-5 and severity-2 incident; verify severity-5 appears first.
4. Dispatch; verify compatible ambulance/hospital assignment and route estimates.
5. Complete the incident; verify ambulance is released and reserved bed count is restored.
6. Restart the app; verify incident records reload from `incidents.csv`.
7. Enter invalid latitude/longitude or invalid severity; verify input is rejected.
8. Enter coordinates more than 5 km from the demo graph; verify the system refuses to invent a route.
9. Verify `dispatch_audit.log` records registration, dispatch, severity changes, cancellation and completion.
10. Review modules: IncidentRules centralizes validation/status policy; RouteAnalytics estimates time and formats paths; ResourceManager owns fleet/hospital eligibility; IncidentReporter owns output; DispatchAudit owns audit writes.
