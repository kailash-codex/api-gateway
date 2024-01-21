import { bootstrapApplication } from '@angular/platform-browser';
import { DashboardComponent } from './app/dashboard/dashboard.component';
import { AppComponent } from './app/app.component';
import { config } from './app/app.config.server';

const bootstrap = () => bootstrapApplication(DashboardComponent, config);

export default bootstrap;
